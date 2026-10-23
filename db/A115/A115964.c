/*
 * A115964  Denominator of Sum_{i=1..n} 1/prime(i)^3.
 * https://oeis.org/A115964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = (product of the first n primes)^3
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   get_prime, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_mul.h"
#include "../utils/get_prime.h"

/* Sequence A115964: denominator of Sum_{i=1..n} 1/prime(i)^3
 * Since the denominator is the primorial cubed, a(n)=P_n^3,
 * where P_n = prime_1 * prime_2 * … * prime_n. */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n==0 we return 1
       as a harmless placeholder, but this value is never used. */
    if (n == 0) return (seq_t)1;

    seq_t prod = (seq_t)1;          /* product of first i-1 primes   */
    for (idx_t i = 1; i <= n; ++i) {
        uint32_t p = get_prime(i); /* i‑th prime (prime_1 is 2)   */
        prod = seq_mul(prod, (seq_t)p);
    }
    /* cube prod : prod^3 = prod * (prod * prod) */
    seq_t prod2 = seq_mul(prod, prod);
    return seq_mul(prod2, prod);
}
