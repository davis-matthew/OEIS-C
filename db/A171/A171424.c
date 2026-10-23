/*
 * A171424  (n-1)^(p-n+1)+n where p is the smallest prime > n-1.
 * https://oeis.org/A171424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)^(p-n+1) + n, where p is the smallest prime > n-1
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   big_pow, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined for n=0, guard against underflow */

    idx_t m = n - 1;          /* base index (n-1) */
    idx_t p = m + 1;          /* candidate for next prime > m */

    while (!is_prime(p))
        ++p;                   /* locate the smallest prime greater than m */

    seq_t base  = (seq_t)m;           /* (n-1) as seq_t */
    seq_t exp   = (seq_t)(p - n + 1); /* exponent: p - n + 1 */
    seq_t pow_val = big_pow(base, exp); /* compute (n-1)^(p-n+1) */
    return pow_val + (seq_t)n;         /* add n and return the result */
}
