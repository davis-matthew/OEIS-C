/*
 * A214521  List of words over {1,4} with equal numbers of 1's and 4's.
 * https://oeis.org/A214521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumerate words over {1,4} with equal numbers of 1's and 4's in lexicographic order.
   a(1) = 14, a(2) = 41, a(3) = 1144, …   (OEIS A214521) */

#include <stdint.h>

/* Return binomial coefficient C(n,k) using 64‑bit arithmetic. */
static idx_t binom_idx(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    idx_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = res * (n - k + i) / i;
    return res;
}

seq_t a(idx_t n)
{
    /* Find the block length 2*k containing the nth word. */
    idx_t target = n;
    idx_t cum = 0;
    idx_t k = 0;
    for (;;)
    {
        ++k;
        idx_t c = binom_idx(2 * k, k);
        if (cum + c >= target)
            break;
        cum += c;
    }

    /* Rank of the word within this block (1‑based). */
    idx_t rank = target - cum;

    /* Unrank the lexicographic word of length 2*k. */
    idx_t remaining = 2 * k;  /* still to place */
    idx_t ones = k;           /* number of '1's still left to put */

    seq_t result = 0;
    while (remaining > 0)
    {
        idx_t cnt1 = 0;
        if (ones > 0)
            cnt1 = binom_idx(remaining - 1, ones - 1);

        if (rank <= cnt1)
        {
            result = result * (seq_t)10 + (seq_t)1;
            --ones;
        }
        else
        {
            result = result * (seq_t)10 + (seq_t)4;
            rank -= cnt1;
        }
        --remaining;
    }

    return result;
}
