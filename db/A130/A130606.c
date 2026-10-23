/*
 * A130606  a(n) = prime(n+1)^n - prime(n)^n where prime(n) is the n-th prime
 *          number.
 * https://oeis.org/A130606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n+1)^n - prime(n)^n
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses the utilities get_prime and big_pow to compute the required terms. */

#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Ensure we have valid input; for n < 1 the behavior is undefined. */
    uint32_t p_n   = get_prime(n);     /* prime(n)   */
    uint32_t p_n1  = get_prime(n + 1); /* prime(n+1) */

    /* Compute prime(n+1)^n and prime(n)^n */
    seq_t pow1 = big_pow((seq_t)p_n1, (seq_t)n);
    seq_t pow0 = big_pow((seq_t)p_n,   (seq_t)n);

    /* a(n) = prime(n+1)^n - prime(n)^n */
    return pow1 - pow0;
}
