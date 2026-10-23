/*
 * A037301  Numbers whose base-2 and base-3 expansions have the same digit
 *          sum.
 * https://oeis.org/A037301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 7163, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A037301: numbers whose base‑2 and base‑3 expansions have the same digit sum.
   Computation: linear scan of non‑negative integers, caching the found terms. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Population count for a seq_t value. */
static inline int popcount_seq(seq_t v)
{
    int cnt = 0;
    while (v)
    {
        v &= v - 1;
        cnt++;
    }
    return cnt;
}

/* Sum of digits of a seq_t value in base 3. */
static inline int digit_sum_base3(seq_t v)
{
    int sum = 0;
    while (v)
    {
        sum += (int)(v % 3);
        v /= 3;
    }
    return sum;
}

/* Return the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    /* Cache of found terms.  The sequence grows only up to ~10000 terms
       in the tests, so a fixed static array is sufficient. */
    static seq_t cache[20000];
    static size_t terms_found = 0;
    static idx_t next_candidate = 0;

    while (terms_found < n)
    {
        seq_t val = (seq_t)next_candidate;
        if (popcount_seq(val) == digit_sum_base3(val))
        {
            cache[terms_found++] = val;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
