/*
 * A085405  Common residues of binomial(3n+2,n+1)/(3n+2) modulo 2.
 * https://oeis.org/A085405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binom(3n+2, n+1)/(3n+2) (Mod 2) – compute parity of the
 *            integer C(3n+2, n+1)/(3n+2) by using 2‑adic valuations: a(n)=1
 *            iff v₂(C(3n+2, n+1)) equals v₂(3n+2)
 * Range:     u8 n <= 9704, u16 n <= 9704, u32 n <= 9704, u64 n <= 9704,
 *            u128 n <= 9704, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* v2_factorial(m) returns the exponent of 2 in m!   */
static unsigned long long v2_factorial(idx_t m)
{
    unsigned long long e = 0;
    idx_t t = m;
    while (t) {
        t >>= 1;
        e += t;
    }
    return e;
}

seq_t a(idx_t n)
{
    /*  N = 3n+2   K = n+1   NK = 2n+1   */
    idx_t N  = 3*n + 2;
    idx_t K  = n + 1;
    idx_t NK = N - K;          /* 2n+1 */

    /* 2‑adic valuation of the binomial coefficient */
    unsigned long long e_binom = v2_factorial(N) - v2_factorial(K) - v2_factorial(NK);

    /* 2‑adic valuation of the denominator 3n+2 */
    unsigned long long e_den = 0;
    idx_t d = N;
    while ((d & 1ULL) == 0) {
        e_den++;
        d >>= 1;
    }

    return (e_binom == e_den) ? (seq_t)1 : (seq_t)0;
}
