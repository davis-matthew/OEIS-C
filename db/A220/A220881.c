/*
 * A220881  Number of nonequivalent dissections of an n-gon into n-3 polygons
 *          by nonintersecting diagonals up to rotation.
 * https://oeis.org/A220881
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(2n-4)) * Sum_{d | (2n-4, n)} phi(d) *
 *            binomial((2n-4)/d, n/d)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute φ(m) for m <= 2*n-4, where n <= 10000, so m fits in 64‑bit. */
static seq_t phi_uint64(uint64_t m)
{
    uint64_t result = m;
    uint64_t d = m;
    for (uint64_t p = 2; p * p <= d; ++p) {
        if (d % p == 0) {
            while (d % p == 0) d /= p;
            result -= result / p;
        }
    }
    if (d > 1) result -= result / d;
    return (seq_t)result;
}

/* Compute binomial(n,k) multiplicatively. */
static seq_t binomial(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return res;
}

/* Return sequence value for a(n). */
seq_t a(idx_t n)
{
    if (n < 4) return 0;

    idx_t two_n_minus_four = 2 * n - 4;

    /* Compute g = gcd(two_n_minus_four, n) */
    idx_t a_val = two_n_minus_four, b_val = n;
    while (b_val != 0) {
        idx_t t = a_val % b_val;
        a_val = b_val;
        b_val = t;
    }
    idx_t g = a_val; /* gcd */

    seq_t sum = 0;
    for (idx_t i = 1; (uint64_t)i * (uint64_t)i <= (uint64_t)g; ++i) {
        if (g % i == 0) {
            idx_t d = i;
            seq_t phi_d = phi_uint64(d);
            seq_t bi = binomial((idx_t)((two_n_minus_four) / d), (idx_t)(n / d));
            sum += phi_d * bi;

            if (i != g / i) {
                d = g / i;
                phi_d = phi_uint64(d);
                bi = binomial((idx_t)((two_n_minus_four) / d), (idx_t)(n / d));
                sum += phi_d * bi;
            }
        }
    }

    return sum / (seq_t)two_n_minus_four;
}
