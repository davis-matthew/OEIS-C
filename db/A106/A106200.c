/*
 * A106200  a(n)=denominator of the probability that
 *          (x-y)/(x+y)+(y-z)/(y+z)+(z-u)/(z+u)+ (u-x)/(u+x) >0, assuming that
 *          each random quadruple of integers (x,y,z,u), with a<=x,y,z,u<=n,
 *          is equally likely.
 * https://oeis.org/A106200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = denominator of
 *            [n(n-1)(n^2-n-1)+4∑_{k=2}^{⌊√n⌋}⌊n/k²⌋ϕ(k)-2∑_{k=2}^{n}⌊n/k⌋²ϕ(k)]/(2n⁴)
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the reduced denominator of the probability that  
   (x-y)/(x+y)+(y-z)/(y+z)+(z-u)/(z+u)+(u-x)/(u+x) > 0  
   for a uniformly random quadruple (x,y,z,u) with 1 ≤ x,y,z,u ≤ n. */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#if defined(__SIZEOF_INT128__)
  typedef unsigned __int128  big_t;
#else
  /* Fallback to 64‑bit if 128‑bit not available; works for most test cases. */
  typedef unsigned long long big_t;
#endif

/* Euclidean algorithm for big_t.  Works for 64‑bit or 128‑bit `big_t`. */
static inline big_t gcd128(big_t a, big_t b) {
    while (b) {
        big_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    /* Lazy, growing sieve for Euler's totient up to n. */
    static seq_t *phi = NULL;
    static idx_t phi_size = 0;

    if (n > phi_size) {
        /* Reallocate phi array to accommodate up to n (inclusive). */
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(phi, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed – return 0 */
        phi = tmp;
        /* Initialize phi[i] = i for i ≥ 0. */
        for (idx_t i = 0; i <= n; ++i) phi[i] = (seq_t)i;
        if (n >= 1) phi[1] = (seq_t)1;
        else phi[0] = (seq_t)0;
        /* Sieve to compute φ. */
        for (idx_t i = 2; i <= n; ++i) {
            if (phi[i] == (seq_t)i) {        /* i is prime */
                for (idx_t j = i; j <= n; j += i) {
                    phi[j] = (seq_t)((seq_t)phi[j] - ((seq_t)phi[j] / (seq_t)i));
                }
            }
        }
        phi_size = n;
    }

    /* Convert to big_t for intermediate calculations. */
    big_t n_big = (big_t)n;

    /* n^4 and 2*n^4. */
    big_t n2 = n_big * n_big;
    big_t n4 = n2 * n2;
    big_t two_n4 = (big_t)2 * n4;

    /* Main polynomial term: n(n-1)(n^2-n-1). */
    big_t term_main = n_big * (n_big - (big_t)1) *
                      (n2 - n_big - (big_t)1);

    /* Sum1: 4 * Σ ⌊n/k²⌋ * φ(k), k=2..⌊√n⌋ */
    big_t sum1 = (big_t)0;
    idx_t limit = (idx_t)floor(sqrt((double)n));
    for (idx_t k = 2; k <= limit; ++k) {
        idx_t denom = k * k;
        idx_t f = n / denom;
        if (f == 0) break;
        sum1 += (big_t)phi[k] * (big_t)f;
    }
    big_t term1 = (big_t)4 * sum1;

    /* Sum2: 2 * Σ ⌊n/k⌋² * φ(k), k=2..n */
    big_t sum2 = (big_t)0;
    for (idx_t k = 2; k <= n; ++k) {
        idx_t f = n / k;
        big_t sq = (big_t)f * (big_t)f;
        sum2 += (big_t)phi[k] * sq;
    }
    big_t term2 = (big_t)2 * sum2;

    /* Numerator of the fraction. */
    big_t numerator = term_main + term1 - term2;

    /* Reduce the fraction and return the reduced denominator. */
    big_t g = gcd128(numerator, two_n4);
    big_t denom_reduced = two_n4 / g;

    return (seq_t)denom_reduced;
}
