/*
 * A184883  Number triangle T(n,k) = [k<=n]*Hypergeometric2F1([-k, 2k-2n],
 *          [1], 2).
 * https://oeis.org/A184883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,k) where * r = floor((sqrt(8*n + 1) - 1) / 2) * k = n
 *            - r*(r+1) / 2 * T(r,k) = Sum_{j=0..k} C(2r - 2k, j) * C(k, j) *
 *            2^j * This is the row‑major flattening of the triangle T(n,k) of
 *            OEIS A184883
 * Range:     u8 n <= 23, u16 n <= 70, u32 n <= 219, u64 n <= 798,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

/* Compute binomial coefficient C(n,k) in a multiplicative way. */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* Compute T(r,k) using the summation formula. */
static seq_t triangle_value(idx_t r, idx_t k)
{
    seq_t sum  = 0;
    seq_t pow2 = 1;                    /* 2^0 */
    idx_t two_r_minus_2k = 2 * r - 2 * k;
    for (idx_t j = 0; j <= k; ++j) {
        seq_t b1   = binom((idx_t)two_r_minus_2k, j);
        seq_t b2   = binom(k, j);
        seq_t term = b1 * b2 * pow2;
        sum += term;
        pow2 <<= 1;                    /* multiply by 2 for next j */
    }
    return sum;
}

/* Main function requested by the build system. */
seq_t a(idx_t n)
{
    /* Convert n to long double for sqrt.  For n up to about 10^6, double
       precision is sufficient.  For larger n the sequence is still
       well‑defined but 8*n+1 fits in 64 bits, so long double is safe. */
    long double val = sqrtl((long double)8 * n + 1.0L);
    idx_t r = (idx_t)((val - 1.0L) / 2.0L);          /* floor((sqrt-1)/2) */
    idx_t triangular = r * (r + 1) / 2;
    idx_t k = n - triangular;
    return triangle_value(r, k);
}
