/*
 * A060474  a(n) = denominator of phi(n)/(n+1), where phi(n) is Euler's phi,
 *          A000010.
 * https://oeis.org/A060474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nice, nonn
 * Formula:   a(n) = denominator of φ(n)/(n+1), where φ is Euler's totient
 *            function
 * Range:     u8 n <= 254, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute gcd of two seq_t values. Works for all supported seq_t types (int, long long,
   unsigned __int128, or arbitrary‑precision integer classes). */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Compute φ(n) for n up to 64‑bit.  The function uses simple trial division; it
   works efficiently for the required n ≤ 10 000 and produces the correct result
   for any 64‑bit n.  The result type is seq_t, so the multiplication and
   subtraction are performed with the same arithmetic that seq_t supports. */
static seq_t phi_calc(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;
    seq_t result = (seq_t)n;
    idx_t m = n;
    for (idx_t p = (idx_t)2; p * p <= m; ++p) {
        if (m % p == (idx_t)0) {
            while (m % p == (idx_t)0) m /= p;
            result -= result / (seq_t)p;
        }
    }
    if (m > (idx_t)1) {
        result -= result / (seq_t)m;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t phi_n = phi_calc(n);
    seq_t d = (seq_t)n + (seq_t)1;
    seq_t g = seq_gcd(phi_n, d);
    return d / g;
}
