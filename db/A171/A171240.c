/*
 * A171240  a(n) = n^d+d^n where d = A013632(n) is the distance to the next
 *          prime.
 * https://oeis.org/A171240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^d + d^n, where d = A013632(n) = next_prime(n) - n
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 18, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   big_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* for n = 0 the sequence is undefined; return 0 */
    if (n == 0)
        return 0;

    /* distance to the next prime greater than n */
    uint64_t p = next_prime(n);
    uint64_t d = p - n;

    /* convert to seq_t for big integer arithmetic */
    seq_t sn = (seq_t)n;
    seq_t sd = (seq_t)d;

    /* compute n^d and d^n */
    seq_t term1 = big_pow(sn, sd);  /* n^d */
    seq_t term2 = big_pow(sd, sn);  /* d^n */

    return term1 + term2;
}
