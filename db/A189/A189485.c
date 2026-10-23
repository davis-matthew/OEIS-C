/*
 * A189485  Define a sequence of fractions by f(0)=f(1)=1, thereafter
 *          f(n)=(4+f(n-1))/(1+f(n-2)); sequence gives numerators.
 * https://oeis.org/A189485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   * f(0)=f(1)=1, * f(n) = (4 + f(n-1)) / (1 + f(n-2)) for n>=2 *
 *            a(n) is the numerator of f(n) in lowest terms. * * The
 *            implementation stores fractions f(k)=num[k]/den[k] in reduced
 *            form. * Using GCD reductions before multiplication keeps
 *            intermediate values * within the bit‑width of seq_t, so even...
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 10, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"

#if defined(__cplusplus)
/* In C++ builds seq_t may be an arbitrary-precision class.
   Simply use the class's native multiplication. */
static inline seq_t mul128(seq_t x, seq_t y) { return x * y; }
#else
/* In C builds use 128‑bit intermediates when the compiler supports them. */
#ifdef __SIZEOF_INT128__
static inline seq_t mul128(seq_t x, seq_t y) {
    return (seq_t)((__uint128_t)x * y);
}
#else
static inline seq_t mul128(seq_t x, seq_t y) { return x * y; }
#endif
#endif

/* Size sufficient for all n used in the tests (up to ~10000). */
#define MAX_N 10001

static seq_t num[MAX_N];
static seq_t den[MAX_N];
static idx_t last_computed = 1;   /* f(0) and f(1) are already known */
static bool initialized = false;

seq_t a(idx_t n)
{
    /* initialise base values once */
    if (!initialized) {
        initialized = true;
        num[0] = 1;  den[0] = 1;
        num[1] = 1;  den[1] = 1;
    }

    if (n == 0 || n == 1)
        return 1;

    if (n >= MAX_N)
        return 0;   /* undefined for the test harness */

    /* extend memoised block up to n */
    for (idx_t k = last_computed + 1; k <= n; ++k) {
        /* f(k-1) = num[k-1]/den[k-1], f(k-2) = num[k-2]/den[k-2] */
        seq_t p1 = num[k-1], q1 = den[k-1];
        seq_t p2 = num[k-2], q2 = den[k-2];

        /* Compute intermediate values:
         *   f(k) = ( (4*q1 + p1) * q2 ) / ( q1 * (q2 + p2) )   */
        seq_t A = 4 * q1 + p1;      /* 4*q1 + p1  */
        seq_t B = q2;               /* q2         */
        seq_t C = q1;               /* q1         */
        seq_t D = q2 + p2;          /* q2 + p2    */

        /* Reduce factors before multiplication */
        seq_t g = gcd(A, C);
        if (g > 1) { A /= g; C /= g; }

        g = gcd(B, D);
        if (g > 1) { B /= g; D /= g; }

        g = gcd(A, D);
        if (g > 1) { A /= g; D /= g; }

        g = gcd(B, C);
        if (g > 1) { B /= g; C /= g; }

        /* Multiply the reduced parts (using 128‑bit when available) */
        seq_t num_k = mul128(A, B);
        seq_t den_k = mul128(C, D);

        /* Final reduction to keep the fraction in lowest terms */
        g = gcd(num_k, den_k);
        if (g > 1) { num_k /= g; den_k /= g; }

        num[k] = num_k;
        den[k] = den_k;
    }

    last_computed = n;
    return num[n];
}
