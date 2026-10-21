/*
 * A079094  Arithmetic derivative of (prime(n)+1)*(prime(n+1)+1)/4.
 * https://oeis.org/A079094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Arithmetic derivative of (prime(n)+1)*(prime(n+1)+1)/4 a(n) =
 *            A003415( (prime(n)+1)*(prime(n+1)+1)/4 )
 * Range:     u8 n <= 6, u16 n <= 69, u32 n <= 5660, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/get_prime.h"

static seq_t arithmetic_derivative(uint64_t m)
{
    seq_t sum = 0;
    uint64_t tmp = m;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            sum += (seq_t)(m / p) * (seq_t)e;
        }
    }
    if (tmp > 1) {    /* remaining prime factor */
        sum += (seq_t)(m / tmp);
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* get consecutive primes */
    uint32_t pn  = get_prime(n);
    uint32_t pn1 = get_prime(n + 1);

    /* compute m = (p_n+1)*(p_{n+1}+1)/4 as a seq_t */
    seq_t prod = (seq_t)(pn + 1) * (seq_t)(pn1 + 1);
    prod /= (seq_t)4;             /* exact division */

    /* convert to uint64_t for factorisation (value fits in 64 bits) */
    uint64_t m = (uint64_t)prod;
    return arithmetic_derivative(m);
}
