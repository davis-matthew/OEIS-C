/*
 * A140893  a(n) = prime(n)^prime(n+1) - prime(n+1)^prime(n).
 * https://oeis.org/A140893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = prime(n)^prime(n+1) - prime(n+1)^prime(n)
 * Range:     i8 n <= 2, i16 n <= 2, i32 n <= 4, i64 n <= 6, i128 n <= 8,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* OEIS offset: a(1) is first term */
    if (n < 1) {
        return (seq_t)0; /* undefined for n < 1 */
    }
    uint32_t p_n   = get_prime(n);
    uint32_t p_np1 = get_prime(n + 1);
    seq_t prime_n   = (seq_t)p_n;
    seq_t prime_np1 = (seq_t)p_np1;

    /* big_pow handles large exponents; works for both arbitrary-precision
       and fixed-width types. */
    seq_t pow1 = big_pow(prime_n,   prime_np1); /* prime(n)^prime(n+1) */
    seq_t pow2 = big_pow(prime_np1, prime_n);   /* prime(n+1)^prime(n)   */

    return pow1 - pow2;
}
