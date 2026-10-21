/*
 * A052716  Expansion of e.g.f. (x + 1 - sqrt(1-6*x+x^2))/2.
 * https://oeis.org/A052716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) satisfies (n^2-1)*a(n) = (3+6n)*a(n+1) - a(n+2), with
 *            a(0)=0, a(1)=2, a(2)=4 (derived from the egf)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;

    /* Maintain a(n-1) and a(n) to compute a(n+1) iteratively. */
    seq_t a_n_minus_one = (seq_t)2; /* a(1) */
    seq_t a_n           = (seq_t)4; /* a(2) */
    seq_t a_next;
    for (idx_t i = 1; i <= n - 2; ++i) {
        /* a_next = a(i+2) */
        seq_t term1 = (seq_t)(3 + 6 * i) * a_n;
        seq_t term2 = (seq_t)(i * i - 1) * a_n_minus_one;
        a_next = term1 - term2;
        a_n_minus_one = a_n;
        a_n = a_next;
    }
    return a_n;
}
