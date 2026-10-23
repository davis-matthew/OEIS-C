/*
 * A115878  a(n) is the number of positive solutions of the Diophantine
 *          equation x^2 = y(y+n).
 * https://oeis.org/A115878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of positive solutions of x² = y(y+n). If n = 2^m ·
 *            o where o is odd, let τ(o²) be the number of divisors of o².
 *            Then a(o) = (τ(o²) – 1) / 2. If m ≤ 2 : a(n) = a(o). If m > 2 :
 *            a(n) = (2m – 3) · a(o) + (m – 2)
 * Range:     u8 n <= 9715, u16 n <= 9715, u32 n <= 9715, u64 n <= 9715,
 *            u128 n <= 9715, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes a(n) for any n ≥ 1.  The function uses only integer arithmetic
   and trial division for factoring the odd part of n.  This is fast enough
   for n ≤ 10⁴ and satisfies the time and memory limits for larger n as
   well.  */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;          /* a(1) = 0, a(0) is never queried */

    /* Split n into 2^m · odd. */
    idx_t temp = n;
    uint64_t m = 0;
    while ((temp & (idx_t)1ULL) == 0) {
        temp >>= 1;
        ++m;
    }
    idx_t odd = temp;

    /* Compute τ(odd²) = ∏ (2·e_i + 1). */
    seq_t tau = (seq_t)1;
    idx_t remaining = odd;
    /* Factor 3,5,7,... up to sqrt(remaining). */
    for (idx_t p = 3; p <= remaining / p; p += 2) {
        if (remaining % p == 0) {
            unsigned exp = 0;
            while (remaining % p == 0) {
                remaining /= p;
                ++exp;
            }
            tau *= (seq_t)(2 * exp + 1);
        }
    }
    if (remaining > 1) {
        /* remaining is prime (exponent 1). */
        tau *= (seq_t)3;              /* 2*1+1 = 3 */
    }

    /* a(odd) = (τ(odd²) – 1) / 2. */
    seq_t a_odd = (tau - (seq_t)1) / (seq_t)2;

    if (m <= 2)
        return a_odd;

    /* a(n) = (2m – 3)·a(odd) + (m – 2). */
    seq_t res = ((seq_t)2 * (seq_t)m - (seq_t)3) * a_odd
                + ((seq_t)m - (seq_t)2);
    return res;
}
