/*
 * A171904  Smallest number m such that m and m+n have both at least two
 *          identical neighboring digits in their decimal representations.
 * https://oeis.org/A171904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) = smallest m such that m and m+n both contain an adjacent
 *            pair of equal digits
 * Range:     u8 n <= 398, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true iff the decimal representation of x has at least one pair of equal neighboring digits. */
static bool has_adjacent(seq_t x)
{
    if (x < 10) return false;                      /* single digit – impossible */
    seq_t prev = x % 10;                           /* least significant digit */
    x /= 10;
    while (x > 0)
    {
        seq_t cur = x % 10;                        /* next digit */
        if (cur == prev) return true;              /* duplicate pair found */
        prev = cur;
        x /= 10;
    }
    return false;                                  /* no duplicate pair */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                          /* offset starts at 1 – safe return for 0 */
    seq_t m = (seq_t)1;
    seq_t n_seq = (seq_t)n;
    while (true)
    {
        if (has_adjacent(m) && has_adjacent(m + n_seq))
            return m;
        ++m;
    }
}
