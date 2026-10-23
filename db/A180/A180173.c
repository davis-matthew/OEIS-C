/*
 * A180173  a(n) = gcd(prime(n)-2, n).
 * https://oeis.org/A180173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(prime(n)-2, n)
 * Range:     u8 n <= 56, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   get_prime, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_gcd.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* safety guard – the sequence is defined for n ≥ 1 */
        return (seq_t)0;

    seq_t prime_val   = (seq_t)get_prime(n);  /* prime(n) */
    seq_t prime_minus = prime_val - 2;        /* prime(n) - 2   */
    seq_t n_val       = (seq_t)n;             /* the index n    */

    return seq_gcd(prime_minus, n_val);
}
