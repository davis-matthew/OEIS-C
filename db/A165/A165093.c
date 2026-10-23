/*
 * A165093  (Length of preperiodic part) + (length of cycle) of trajectory of
 *          n under iteration of the base-8 Kaprekar map in A165090.
 * https://oeis.org/A165093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = Length of preperiodic part + length of cycle of the
 *            trajectory of n under the base‑8 Kaprekar map (A165090)
 * Range:     u8 n <= 65, u16 n <= 4096, u32 n <= 4096, u64 n <= 4096,
 *            u128 n <= 4096, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Utility: comparison function for qsort on unsigned 8‑bit digits    */
/* ------------------------------------------------------------------ */
static int cmp_uint8(const void *a, const void *b)
{
    uint8_t ca = *(const uint8_t*)a;
    uint8_t cb = *(const uint8_t*)b;
    return (ca > cb) - (ca < cb);
}

/* ------------------------------------------------------------------ */
/* Compute one step of the base‑8 Kaprekar map for value x            */
/* ------------------------------------------------------------------ */
static seq_t kaprekar_oct(seq_t x)
{
    /* Extract octal digits of x */
    uint8_t digits[256];   /* generous size for safety */
    size_t dlen = 0;
    seq_t t = x;
    if (t == 0) {
        digits[dlen++] = 0;
    } else {
        while (t != 0) {
            digits[dlen++] = (uint8_t)(t % 8);
            t = t / 8;
        }
    }

    /* Sort digits ascending */
    qsort(digits, dlen, sizeof(uint8_t), cmp_uint8);

    /* Build smallest number */
    seq_t smallest = 0;
    for (size_t i = 0; i < dlen; ++i) {
        smallest = smallest * 8 + digits[i];
    }

    /* Build largest number (digits in reverse order) */
    seq_t largest = 0;
    for (size_t i = dlen; i > 0; --i) {
        largest = largest * 8 + digits[i - 1];
    }

    return largest - smallest;
}

/* ------------------------------------------------------------------ */
/* Helper: grow a dynamic array of seq_t, constructing new objects
   correctly in C++ and copying in C.                                */
/* ------------------------------------------------------------------ */
static void grow_seq_array(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap >= needed)
        return;

    size_t new_cap = *cap ? *cap * 2 : 4;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *new_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_arr)
        return; /* allocation failure; caller will handle */

    /* Copy existing elements, constructing them if needed */
    for (size_t i = 0; i < *cap; ++i) {
#if defined(__cplusplus)
        new (&new_arr[i]) seq_t((*arr)[i]);   /* copy constructor */
#else
        new_arr[i] = (*arr)[i];
#endif
    }

    /* Destroy old objects if C++ */
#if defined(__cplusplus)
    for (size_t i = 0; i < *cap; ++i) {
        (*arr)[i].~seq_t();
    }
#endif

    free(*arr);
    *arr = new_arr;
    *cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    static seq_t *seen = NULL;
    static size_t cap = 0;

    /* Initialize storage once */
    if (!seen) {
        cap = 4;
        seen = (seq_t*)malloc(cap * sizeof(seq_t));
    }

    seq_t x = (seq_t)n;
    size_t len = 1;
    seen[0] = x;

    while (1) {
        seq_t next = kaprekar_oct(x);

        /* linear search for duplicate */
        size_t i;
        for (i = 0; i < len; ++i) {
            if (seen[i] == next)
                break;
        }
        if (i < len)               /* duplicate found → cycle ends */
            break;

        /* ensure capacity for the new element */
        grow_seq_array(&seen, &cap, len + 1);

        seen[len++] = next;
        x = next;
    }

    return (seq_t)len;
}
