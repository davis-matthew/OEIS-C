/*
 * A023787  Katadromes: digits in base 5 are in strict descending order.
 * https://oeis.org/A023787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   a(n) = the nth smallest integer whose base‑5 representation has
 *            strictly decreasing digits (no repeats)
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Compare two seq_t values for qsort. */
static int cmp_seq(const void *p, const void *q)
{
    seq_t a = *(const seq_t *)p;
    seq_t b = *(const seq_t *)q;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0)        /* offset is 1, so n<1 is undefined */
        return 0;

    /* There are exactly 31 numbers with strictly decreasing base‑5 digits. */
    static seq_t cache[31];
    static bool initialized = false;

    if (!initialized)
    {
        size_t k = 0;
        /* Enumerate all non‑empty subsets of {0,…,4}. */
        for (idx_t mask = 1; mask < ((idx_t)1 << 5); ++mask)
        {
            seq_t val = 0;
            /* Build the number from most significant digit to least
               by traversing the bitmask from high to low. */
            for (int pos = 4; pos >= 0; --pos)
            {
                if (mask & ((idx_t)1 << pos))
                    val = val * 5 + (seq_t)pos;
            }
            cache[k++] = val;          /* k will become 31 */
        }
        qsort(cache, k, sizeof(seq_t), cmp_seq);
        initialized = true;
    }

    /* Return the requested term, or 0 if the index exceeds the length. */
    if (n - 1 >= 31)
        return 0;
    return cache[n - 1];
}
