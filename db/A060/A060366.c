/*
 * A060366  Powers of one thousand which are described by single words in
 *          dated British English usage, extended by using "-ard" beyond 10^9.
 * https://oeis.org/A060366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, nonn, word
 * Formula:   a(n) = 10^(3*(n-1))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* a(1) = 10^0 = 1; for safety return 0 for n=0. */
    if (n == 0)
        return 0;
    /* Compute exponent 3·(n−1).  All calculations use unsigned 64‑bit
       arithmetic which is sufficient for the ranges used in the tests. */
    idx_t exp = 3 * (n - 1);
    return pow10(exp);
}
