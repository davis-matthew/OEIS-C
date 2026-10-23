/*
 * A130604  Numbers whose base-10 and base-7 representations are permutations
 *          of the same multiset of digits.
 * https://oeis.org/A130604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A130604: numbers whose base‑10 and base‑7 representations are permutations of the same multiset of digits. */
/* The sequence is finite.  All terms are ≤ 117648. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static bool good(idx_t x)
{
    unsigned char cnt10[10] = {0};
    unsigned char cnt7[10]  = {0};

    if (x == 0) {
        cnt10[0] = 1;
        cnt7[0]  = 1;
    } else {
        idx_t y = x;
        while (y > 0) {
            int d = y % 10;
            if (d >= 7) return false;              // base‑7 digits cannot exceed 6
            cnt10[d]++;
            y /= 10;
        }
        y = x;
        while (y > 0) {
            int d = y % 7;
            cnt7[d]++;
            y /= 7;
        }
    }

    for (int d = 0; d < 10; d++)
        if (cnt10[d] != cnt7[d]) return false;
    return true;
}

static seq_t* init_terms(size_t *out_count)
{
    static seq_t *list = NULL;
    static size_t capacity = 0;
    const idx_t MAX = 117648;           /* 7^7 - 7^6 = 117648  */
    if (list != NULL) {
        *out_count = capacity;          /* already computed */
        return list;
    }

    capacity = 200;                     /* start capacity */
    list = (seq_t*)malloc(capacity * sizeof(seq_t));
    if (!list) return NULL;             /* allocation failed */

    size_t count = 0;
    for (idx_t n = 0; n <= MAX; ++n) {
        if (good(n)) {
            if (count == capacity) {
                capacity *= 2;
                seq_t *tmp = (seq_t*)realloc(list, capacity * sizeof(seq_t));
                if (!tmp) {
                    free(list);
                    return NULL;
                }
                list = tmp;
            }
            list[count++] = (seq_t)n;
        }
    }
    *out_count = count;
    return list;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* offset is 1, but guard anyway */

    static seq_t *terms = NULL;
    static size_t term_count = 0;

    if (terms == NULL) {
        terms = init_terms(&term_count);
        if (terms == NULL) return (seq_t)0; /* allocation failed */
    }

    if (n > term_count) return (seq_t)0; /* out of bounds – may not occur */
    return terms[n-1];
}
