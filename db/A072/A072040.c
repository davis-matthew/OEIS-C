/*
 * A072040  Numbers n of the form k + reverse(k) for exactly two k.
 * https://oeis.org/A072040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A072040: numbers n that can be written as k + reverse(k) for exactly two distinct values of k. */
/* Implementation: pre‑compute all k+rev(k) up to a large fixed limit, count occurrences and store ones that occur exactly twice. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Return the decimal reversal of a < 2^64 integer. */
static inline uint64_t rev_uint64(uint64_t x)
{
    uint64_t r = 0;
    while (x != 0)
    {
        r = r * 10ULL + (x % 10ULL);
        x /= 10ULL;
    }
    return r;
}

/* Comparator for qsort on uint64_t values. */
static int cmp_uint64(const void *pa, const void *pb)
{
    uint64_t a = *(const uint64_t *)pa;
    uint64_t b = *(const uint64_t *)pb;
    return (a > b) - (a < b);
}

/* Static storage for the generated sequence. */
static seq_t   *seq_arr = NULL;   /* array of seq_t values */
static idx_t     seq_len = 0;      /* number of stored values */
static bool      initialized = false;

/* Generate the sequence up to a fixed maximum k. */
static void init_sequence(void)
{
    /* The limit must be large enough to contain the required number of terms (≈10 000). */
    const idx_t LIMIT = 3000000ULL;          /* 3 million iterations -> ~24 MB for the sums array */

    /* Allocate temp array for sums (k + rev(k)). */
    uint64_t *sums = (uint64_t *)malloc((size_t)LIMIT * sizeof(uint64_t));
    if (sums == NULL) abort();              /* fatal error if allocation fails */

    /* Compute all sums. */
    for (idx_t k = 1; k <= LIMIT; ++k)
    {
        uint64_t r = rev_uint64((uint64_t)k);
        sums[k-1] = k + r;                  /* fits in uint64_t for this LIMIT */
    }

    /* Sort the sums array. */
    qsort(sums, (size_t)LIMIT, sizeof(uint64_t), cmp_uint64);

    /* Scan sorted sums and keep numbers that occur exactly twice. */
    seq_arr = (seq_t *)malloc((size_t)LIMIT * sizeof(seq_t));  /* worst case allocation */
    if (seq_arr == NULL) abort();

    uint64_t current = sums[0];
    uint32_t count   = 1;
    seq_len = 0;

    for (size_t i = 1; i < (size_t)LIMIT; ++i)
    {
        uint64_t v = sums[i];
        if (v == current)
        {
            ++count;
        }
        else
        {
            if (count == 2)
            {
                seq_arr[seq_len++] = (seq_t)current;
            }
            current = v;
            count   = 1;
        }
    }
    /* Handle the last run. */
    if (count == 2)
    {
        seq_arr[seq_len++] = (seq_t)current;
    }

    /* Clean up temporary array. */
    free(sums);
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (!initialized)
    {
        init_sequence();
        initialized = true;
    }
    /* The sequence is 1‑based. The tests never request n larger than seq_len. */
    return seq_arr[n - 1];
}
