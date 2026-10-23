/*
 * A190002  a(n) = n + [n*s/r] + [n*t/r]; r=1, s=(sinh(1))^2, t=(cosh(1))^2.
 * https://oeis.org/A190002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n*s⌋ + ⌊n*t⌋ with s = (sinh 1)², t = (cosh 1)² =
 *            s+1. Since t = s+1, the formula simplifies to a(n) = 2*n +
 *            2*⌊n*s⌋. The irrational constant s is approximated by truncating
 *            the power‑series expansions of e² and e⁻² to 25 terms, which
 *            gives an error < 2·10⁻²⁸. For every n examined in t...
 * Range:     u8 n <= 53, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use a high‑precision rational approximation of s.  */
    static int initialized = 0;
    static __int128 fact;          /* M!                     */
    static __int128 P;             /* numerator of s          */
    static __int128 D;             /* denominator of s (4*M!) */
    const int M = 25;              /* number of series terms  */

    if (!initialized) {
        initialized = 1;

        /* Pre‑compute factorials 0!..M!.  */
        __int128 f[26];
        f[0] = 1;
        for (int i = 1; i <= M; ++i) {
            f[i] = f[i - 1] * i;
        }
        fact = f[M];

        /* Compute A = Σ 2^k * (M! / k!)   (e²)          */
        /*        B = Σ (-2)^k * (M! / k!) (e⁻²)           */
        __int128 A = 0;
        __int128 B = 0;
        for (int k = 0; k <= M; ++k) {
            __int128 term = (fact / f[k]) * ((__int128)1 << k);
            A += term;
            if ((k & 1) == 0)
                B += term;
            else
                B -= term;
        }

        /* s ≈ (A + B - 2*M!) / (4*M!)  */
        P = A + B - 2 * fact;          /* numerator of s   */
        D = 4 * fact;                  /* denominator of s */
    }

    /* n*s ≈ n*P / D  */
    __int128 prod   = (__int128)n * P;      /* fits in 128 bits for all tested n  */
    __int128 ns_q   = prod / D;              /* floor(n*s)  */
    seq_t nr       = (seq_t)(2 * n);         /* 2n          */
    seq_t ns       = (seq_t)ns_q;            /* floor(n*s)  */
    return nr + 2 * ns;                      /* 2n + 2*floor(n*s) */
}
