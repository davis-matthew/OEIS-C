/*
 * A099210  a(n)=(P(n)*P(n+2))^5 with P(i)=i-th prime.
 * https://oeis.org/A099210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (P(n) * P(n+2))^5 where P(i) is the i-th prime
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   get_prime, seq_mul, seq_pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"   // provides get_prime(idx_t)
#include "../utils/seq_mul.h"     // provides seq_mul(seq_t, seq_t)
#include "../utils/seq_pow_seq.h" // provides seq_pow_seq(seq_t, idx_t)

/* a(n) = (prime(n) * prime(n+2))^5 with offset 1 */
seq_t a(idx_t n)
{
    /* compute the n-th and (n+2)-nd primes */
    seq_t p_n   = get_prime(n);
    seq_t p_n2  = get_prime(n + 2);

    /* multiply them and raise to the 5th power */
    seq_t prod   = seq_mul(p_n, p_n2);
    seq_t result = seq_pow_seq(prod, 5);

    return result;
}
