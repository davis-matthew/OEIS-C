/*
 * A214524  List of words over {1,6} with equal numbers of 1's and 6's.
 * https://oeis.org/A214524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n-th integer in the list of words over {1,6} * with
 *            equal numbers of 1's and 6's, sorted first by increasing length
 *            * and then lexicographically
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 224, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Multiplicative binomial coefficient for small arguments.  */
static seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i)
        r = (r * (seq_t)(n - k + i)) / (seq_t)i;
    return r;
}

seq_t a(idx_t n)
{
    /* Determine k such that the n-th word has length 2k. */
    idx_t k = 1;
    seq_t cumulative = 0;
    seq_t bin = 0;            /* C(2k,k) */
    for (;;)
    {
        if (k == 1)
            bin = 2;          /* C(2,1) */
        else
            bin = (bin * (seq_t)(4 * k - 2)) / (seq_t)k;
        cumulative += bin;
        if (cumulative >= n)
            break;
        ++k;
    }

    seq_t before   = cumulative - bin;          /* total words of shorter lengths */
    seq_t offset   = (seq_t)n - before - 1;     /* 0‑based index within the block */

    /* Unrank the offset‑th word of length 2k with k ones and k sixes. */
    idx_t ones     = k;
    idx_t remaining = 2 * k;
    seq_t value = 0;

    for (idx_t pos = 0; pos < 2 * k; ++pos)
    {
        if (ones == 0)
        {
            value = value * 10 + 6;
            --remaining;
            continue;
        }

        seq_t cnt_if_one = binom_small(remaining - 1, ones - 1);
        if (offset < cnt_if_one)
        {
            value = value * 10 + 1;
            --ones;
        }
        else
        {
            value = value * 10 + 6;
            offset -= cnt_if_one;
        }
        --remaining;
    }
    return value;
}
