/*
 * A058663  a(n) = gcd(n-1, n-phi(n)).
 * https://oeis.org/A058663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n-1, n-phi(n))
 * Range:     u8 n <= 564, u16 n <= 9881, u32 n <= 9881, u64 n <= 9881,
 *            u128 n <= 9881, big any n
 * Helpers:   _phi, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_phi.h"   /* provides _phi(idx_t n) returning seq_t   */
#include "../utils/gcd.h"   /* provides seq_t gcd(seq_t a, seq_t b) */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence value type */
    seq_t n_val = (seq_t)n;

    /* φ(n) */
    seq_t phi_n = _phi(n);

    /* cototient(n) = n - φ(n) */
    seq_t cototient = n_val - phi_n;

    /* n-1 (as unsigned to avoid underflow for n=1) */
    seq_t n_minus_one = n_val - (seq_t)1;

    /* gcd(n-1, cototient(n)) */
    return gcd(n_minus_one, cototient);
}
