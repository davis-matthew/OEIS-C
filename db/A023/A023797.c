/*
 * A023797  Katadromes: digits in base 16 are in strict descending order.
 * https://oeis.org/A023797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, easy
 * Formula:   a(n) = the n‑th smallest integer whose hexadecimal digits are
 *            strictly descending
 * Range:     u8 n <= 17, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Compare two values of type seq_t for qsort. */
static int cmp_seq(const void *a, const void *b)
{
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* There are 2^16 – 1 = 65535 numbers with strictly descending hex digits. */
    static seq_t cache[65535];
    static bool initialized = false;

    if (!initialized)
    {
        const idx_t LIMIT = ((idx_t)1 << 16);          /* 65536 */
        size_t pos = 0;

        /* Generate all numbers from non‑empty subsets of {0,…,15}. */
        for (idx_t mask = 1; mask < LIMIT; ++mask)
        {
            seq_t value = 0;
            for (int d = 15; d >= 0; --d)
            {
                if (mask & ((idx_t)1 << d))
                    value = value * 16 + (seq_t)d;
            }
            cache[pos++] = value;
        }

        /* Sort the numbers increasingly. */
        qsort(cache, pos, sizeof(seq_t), cmp_seq);
        initialized = true;
    }

    /* Offset 1: a(1) = cache[0]. */
    return cache[(size_t)(n - 1)];
}
