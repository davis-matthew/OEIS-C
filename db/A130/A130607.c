/*
 * A130607  a(n) = prime(n+1)^n + prime(n)^n.
 * https://oeis.org/A130607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n+1)^n + prime(n)^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* n is at least 1 by the sequence definition */
    seq_t exp = (seq_t)n;

    /* Retrieve the n‑th and (n+1)‑th primes */
    seq_t p_n   = (seq_t)get_prime(n);
    seq_t p_np1 = (seq_t)get_prime(n + 1);

    /* Compute the powers and add them */
    seq_t pow_n   = big_pow(p_n,   exp);
    seq_t pow_np1 = big_pow(p_np1, exp);

    return pow_n + pow_np1;
}
