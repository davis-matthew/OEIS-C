/*
 * A214532  List of words over {1,9} with equal numbers of 1's and 9's.
 * https://oeis.org/A214532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth word over {1,9} with equal numbers of 1's and
 *            9's, listed in lexicographic order (‘1’ < ‘9’)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for stdint types used internally */

static idx_t binom_u64(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    idx_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (n - k + i) / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Determine the length 2k that contains the nth word */
    idx_t k = 1;
    idx_t cumulative = 0;
    while (1)
    {
        idx_t ways = binom_u64(2 * k, k);
        if (cumulative + ways >= n)
            break;
        cumulative += ways;
        ++k;
    }

    /* 0‑based position within the block of length 2k */
    idx_t pos = n - cumulative - 1;
    idx_t need = k;          /* remaining number of '1's to place */
    seq_t result = 0;
    idx_t length = 2 * k;

    for (idx_t i = 0; i < length; ++i)
    {
        idx_t left = length - i - 1;
        idx_t ways_if_one = 0;
        if (need > 0)
            ways_if_one = binom_u64(left, need - 1);

        if (pos < ways_if_one)
        {
            /* place a '1' */
            result = result * 10 + 1;
            --need;
        }
        else
        {
            /* place a '9' */
            pos -= ways_if_one;
            result = result * 10 + 9;
        }
    }
    return result;
}
