/*
 * A102537  Triangle T(n,k) read by rows: (1/n) * C(2n+k,k-1) * C(n,k); n, k
 *          >= 1.
 * https://oeis.org/A102537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = (1/n) * C(2n+k, k-1) * C(n,k) a(n) = T(r,k) where the
 *            n-th term is read by rows of the triangle
 * Range:     u8 n <= 13, u16 n <= 33, u32 n <= 99, u64 n <= 315,
 *            u128 n <= 1065, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* gcd for 64‑bit integers */
static uint64_t gcd64(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* compute T(r,k) where r>=1 and 1<=k<=r */
static seq_t compute_T(idx_t r, idx_t k)
{
    /* We keep the numerator and denominator factors separately.
       All values in the factors are < 2*r + k <= 2*max_r + max_k < 2*20000,
       so 64‑bit is safe for the factors themselves. */
    uint64_t num[300];
    size_t ncnt = 0U;
    uint64_t den[300];
    size_t dcnt = 0U;

    /* first binomial C(2r+k, k-1) */
    idx_t N1 = 2U * r + k;
    idx_t K1 = k - 1U;
    for (idx_t i = 1U; i <= K1; ++i) {
        num[ncnt++] = (N1 - K1 + i);
        den[dcnt++] = i;
    }

    /* second binomial C(r, k) */
    idx_t N2 = r;
    idx_t K2 = k;
    for (idx_t i = 1U; i <= K2; ++i) {
        num[ncnt++] = (N2 - K2 + i);
        den[dcnt++] = i;
    }

    /* division by r */
    den[dcnt++] = r;

    /* Reduce denominators with numerators */
    for (size_t i = 0U; i < dcnt; ++i) {
        uint64_t d = den[i];
        if (d == 1U) continue;
        for (size_t j = 0U; j < ncnt && d > 1U; ++j) {
            uint64_t g = gcd64(num[j], d);
            if (g > 1U) {
                num[j] /= g;
                d /= g;
            }
        }
        den[i] = d;          /* remaining denominator factor */
    }

    /* Combine remaining numerators, then divide by remaining denominators */
    seq_t result = (seq_t)1U;
    for (size_t j = 0U; j < ncnt; ++j) {
        result = result * (seq_t)num[j];
    }
    for (size_t i = 0U; i < dcnt; ++i) {
        if (den[i] > 1U)
            result = result / (seq_t)den[i];
    }
    return result;
}

seq_t a(idx_t n)
{
    /* offset is 1; ignore n == 0 */
    if (n == 0U) return (seq_t)0U;

    /* find row r such that r(r+1)/2 >= n */
    idx_t r = 1U;
    idx_t tri = 1U;
    while (tri < n) {
        ++r;
        tri = (r * (r + 1U)) / 2U;
    }
    idx_t prev_tri = (r - 1U) * r / 2U;
    idx_t k = n - prev_tri;   /* 1 <= k <= r */

    return compute_T(r, k);
}
