/*
 * A114346  The integer difference between the surface area of the unit sphere
 *          in n-1 dimensions and the volume of the unit sphere in n+1
 *          dimensions.
 * https://oeis.org/A114346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor |S(n-1) – V(n+1)|, where * S(m) = 2*π^((m+1)/2) /
 *            Γ((m+1)/2) * V(m) = π^(m/2) / Γ(m/2 + 1)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

/* Helper: factorial as long double */
static long double fact(unsigned int x)
{
    long double r = 1.0L;
    for (unsigned int i = 2; i <= x; ++i)
        r *= (long double)i;
    return r;
}

/* Helper: Γ(k/2) for integer k */
static long double gamma_k_over_2(idx_t k)
{
    /* k is integer >= 1 */
    const long double pi = 3.141592653589793238462643383279502884L;
    if ((k & 1) == 0) {            /* k even: k = 2*m, Γ(m) = (m‑1)! */
        unsigned int m = (unsigned int)(k / 2);
        if (m == 0)
            return 1.0L;           /* Γ(0) is undefined, but not needed here */
        return fact(m - 1);
    } else {                       /* k odd: k = 2*m+1, Γ(m+0.5) */
        unsigned int m = (unsigned int)((k - 1) / 2);
        long double two_m_fact = fact(2 * m);
        long double sqrt_pi = sqrtl(pi);
        long double denom = powl(4.0L, m) * fact(m);
        return (two_m_fact * sqrt_pi) / denom;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* For n >= 19 the difference is < 1, so floor = 0 */
    if (n >= 19) return (seq_t)0;

    const long double pi = 3.141592653589793238462643383279502884L;

    /* S(n-1) = 2 * π^((n)/2) / Γ(n/2) */
    long double S = 2.0L * powl(pi, (long double)n / 2.0L)
                    / gamma_k_over_2(n);

    /* V(n+1) = π^((n+1)/2) / Γ((n+3)/2) */
    long double V = powl(pi, ((long double)n + 1.0L) / 2.0L)
                    / gamma_k_over_2(n + 3);

    long double diff = S - V;
    if (diff < 0.0L) diff = -diff;

    /* Floor the difference. The added epsilon protects against
     * rounding errors that could put a value just below an integer
     * to the wrong side of the floor. */
    unsigned long long res = (unsigned long long)floorl(diff + 1e-12L);
    return (seq_t)res;
}
