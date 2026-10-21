/*
 * A218171  Expansion of f(x^11, x^13) - x * f(x^5, x^19) in powers of x where
 *          f(, ) is Ramanujan's general theta function.
 * https://oeis.org/A218171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   * a(n) = 1 if n = 12*k^2 + k for some integer k * -1 if n =
 *            12*k^2 + 7*k + 1 for some integer k * 0 otherwise * * This
 *            follows from the closed‐form G.f. description of A218171
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* Integer square root of an unsigned 64‑bit number. */
static unsigned long long isqrt_ull(unsigned long long x)
{
    unsigned long long r = (unsigned long long) sqrt((long double) x);
    while (r * r > x) --r;
    while ((r + 1) * (r + 1) <= x) ++r;
    return r;
}

seq_t a(idx_t n)
{
    /* 1 + 48*n fits easily in 64 bits for all requested n (≤10000). */
    unsigned long long D  = 1ULL + 48ULL * (unsigned long long) n;
    unsigned long long s  = isqrt_ull(D);

    if (s * s != D)          /* discriminant not a perfect square */
        return (seq_t)0;

    /* Candidates for the equation 12*k^2 + k = n   */
    long long k, cand;
    long long sll = (long long) s;

    cand = -1 + sll;
    if (cand % 24 == 0) {
        k = cand / 24;
        if (12LL * k * k + k == (long long) n)
            return (seq_t)1;
    }

    cand = -1 - sll;
    if (cand % 24 == 0) {
        k = cand / 24;
        if (12LL * k * k + k == (long long) n)
            return (seq_t)1;
    }

    /* Candidates for the equation 12*k^2 + 7*k + 1 = n   */
    cand = -7 + sll;
    if (cand % 24 == 0) {
        k = cand / 24;
        if (12LL * k * k + 7LL * k + 1 == (long long) n)
            return (seq_t)-1;
    }

    cand = -7 - sll;
    if (cand % 24 == 0) {
        k = cand / 24;
        if (12LL * k * k + 7LL * k + 1 == (long long) n)
            return (seq_t)-1;
    }

    return (seq_t)0;
}
