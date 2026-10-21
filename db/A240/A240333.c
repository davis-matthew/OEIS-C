/*
 * A240333  Number of n X 2 0..3 arrays with no element equal to two plus the
 *          sum of elements to its left or two plus the sum of elements above
 *          it or one plus the sum of the elements diagonally to its
 *          northwest, modulo 4.
 * https://oeis.org/A240333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{t=0}^{7} c_t * binom(n - t + 4, 5), * where c_t = [4,
 *            -9, 18, -18, 14, -6, -1, 2] and * binom(m, 5) =
 *            m*(m-1)*(m-2)*(m-3)*(m-4) / 120 for m ≥ 5, 0 otherwise
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 166, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Compute binomial with k = 5 using 128‑bit intermediate to avoid overflow. */
static seq_t binom5(idx_t m)
{
    if (m < 5) return 0;
#if defined(__GNUC__) || defined(__clang__) || defined(__INTEL_COMPILER)
    unsigned __int128 prod = (unsigned __int128)m *
                             (unsigned __int128)(m - 1) *
                             (unsigned __int128)(m - 2) *
                             (unsigned __int128)(m - 3) *
                             (unsigned __int128)(m - 4);
    prod /= 120;
    return (seq_t)prod;
#else
    /* Fallback: multiply in stages to keep the product in range of seq_t.
     * This path may overflow for large n when seq_t is 64‑bit, but the
     * reference solver uses the 128‑bit path on supported compilers. */
    seq_t prod = (seq_t)m;
    prod *= (seq_t)(m - 1);
    prod /= 5;          /* keep intermediate smaller */
    prod *= (seq_t)(m - 2);
    prod /= 4;
    prod *= (seq_t)(m - 3);
    prod /= 3;
    prod *= (seq_t)(m - 4);
    prod /= 2;          /* 5! = 5·4·3·2·1 */
    return prod;
#endif
}

seq_t a(idx_t n)
{
    /* coefficients of the numerator polynomial Q(x)   */
    static const int coeffs[8] = {  4, -9, 18, -18, 14, -6, -1,  2 };
    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    /* iterate over the 8 terms of Q(x) */
    for (int t = 0; t < 8; ++t) {
        if (n < (idx_t)t) continue;          /* n - t + 4 would wrap */
        idx_t d = n - (idx_t)t + 4;          /* argument of binom(.,5) */
        seq_t b = binom5(d);
        if (b == 0) continue;                /* coefficient for small n */
        int c = coeffs[t];
        if (c > 0)
            sum_pos += (seq_t)c * b;
        else
            sum_neg += (seq_t)(-c) * b;
    }
    return sum_pos - sum_neg;                /* guaranteed non‑negative */
}
