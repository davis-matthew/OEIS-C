/*
 * A155863  Triangle T(n,k) = n*(n^2 - 1)*binomial(n-2, k-1) for 1 <= k <=
 *          n-1, n >= 2, and T(n,0) = T(n,n) = 1 for n >= 0, read by rows.
 * https://oeis.org/A155863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = linearized triangle with * T(r,0)=T(r,r)=1 *
 *            T(r,k)=r*(r²-1)*binomial(r-2,k-1) for 1≤k≤r-1 * a(n) is the nth
 *            element when the triangle is read row‑by‑row
 * Range:     u8 n <= 16, u16 n <= 59, u32 n <= 308, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* ------------------------- helpers ------------------------- */
static int int_gcd(int a, int b)
{
    while (b) {
        int t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* binomial(n,k) using multiplicative formula (no external link) */
static seq_t binomial_val(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k == 0 || k == n) return (seq_t)1;
    if (k > n - k) k = n - k;

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        uint64_t num = n - k + i;   /* part of the numerator   */
        uint64_t den = i;           /* part of the denominator */

        int g = int_gcd((int)num, (int)den);
        num /= g;
        den /= g;

        res = res * (seq_t)num;
        if (den != 1)
            res = res / (seq_t)den;
    }
    return res;
}

/* ------------------------- main function --------------------- */
seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;

    /* Boundary values */
    if (k == 0 || k == r)
        return (seq_t)1;

    /* Compute T(r,k) = r*(r²-1)*binomial(r-2,k-1) */
    seq_t r_seq = (seq_t)r;
    seq_t term = r_seq * (r_seq * r_seq - (seq_t)1);
    seq_t bin = binomial_val(r - 2, k - 1);
    return term * bin;
}
