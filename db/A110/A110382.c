/*
 * A110382  Numbers which are sum of distinct unary numbers (containing only
 *          ones), i.e., numbers which are sum of distinct numbers of the form
 *          (10^k - 1)/9.
 * https://oeis.org/A110382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = \sum_{k\ge0} popcount(n >> k) * 10^k (Interpret the
 *            binary expansion of n as base‑10 digits)
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 1023, u64 n <= 9498,
 *            u128 n <= 9498, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>            /* for __builtin_popcountll */

/* External types provided by the build */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    seq_t result   = 0;
    seq_t multiplier = 1;          /* 10^k in seq_t */
    idx_t  temp   = n;             /* shift variable of idx_t type */

    while (temp != 0) {
        seq_t pop = (seq_t)__builtin_popcountll(temp);  /* number of 1‑bits */
        result += pop * multiplier;
        temp >>= 1;
        multiplier *= (seq_t)10;
    }

    return result;
}
