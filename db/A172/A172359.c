/*
 * A172359  Triangle read by rows: T(n,k) = round(c(n)/(c(k)*c(n-k))) where c
 *          are partial products of a sequence defined in comments.
 * https://oeis.org/A172359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, less
 * Formula:   a(n) = round( c(n) / (c(k) * c(n-k)) ) where c(0) = 1, c(m) =
 *            ∏_{j=1..m} f(j), f(0) = 0, f(1) = f(2) = 1, f(n) = f(n-2) + 4 *
 *            f(n-3) (for n ≥ 3). The index n is read by rows with offset 0;
 *            a(0) is the first term
 * Range:     u8 n <= 30, u16 n <= 48, u32 n <= 94, u64 n <= 177,
 *            u128 n <= 334, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Find the row r such that offset(r) ≤ n < offset(r+1), where
   offset(r) = r*(r+1)/2.  This works for all 0 ≤ n. */
static inline idx_t find_row(idx_t n)
{
    idx_t low = 0, high = 1;
    while ((high * (high + 1)) / 2 <= n)
        high *= 2;

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((mid * (mid + 1)) / 2 <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;   /* last row with offset ≤ n */
}

/* Main sequence function.  All operations are performed with seq_t. */
seq_t a(idx_t n)
{
    /* ----- Find row r and column k ----- */
    idx_t r = find_row(n);
    idx_t k = n - (r * (r + 1)) / 2;   /* 0 ≤ k ≤ r */

    /* Maximum k for the given n is very small (≤ r ≤ ~2000 for n ≤ 10000). */
    enum { MAX_K = 2000 };
    seq_t numer[MAX_K];
    seq_t denom[MAX_K];

    /* ----- Compute f[0..r] ----- */
    /* f[0] = 0; f[1] = f[2] = 1; f[n] = f[n-2] + 4*f[n-3] */
    seq_t f[MAX_K];
    f[0] = (seq_t)0;
    if (r >= 1) f[1] = (seq_t)1;
    if (r >= 2) f[2] = (seq_t)1;
    for (idx_t i = 3; i <= r; ++i)
        f[i] = f[i - 2] + (seq_t)4 * f[i - 3];

    /* ----- Initialise numerator and denominator arrays ----- */
    for (idx_t i = 0; i < k; ++i) {
        numer[i] = f[r - k + 1 + i];
        denom[i] = f[1 + i];
    }

    /* ----- GCD cancellation to reduce overflow ----- */
    for (idx_t i = 0; i < k; ++i) {
        for (idx_t j = 0; j < k; ++j) {
            seq_t g = gcd(numer[i], denom[j]);
            if (g != (seq_t)1) {
                numer[i] /= g;
                denom[j] /= g;
            }
        }
    }

    /* ----- Multiply remaining factors ----- */
    seq_t num_prod = (seq_t)1;
    seq_t denom_prod = (seq_t)1;
    for (idx_t i = 0; i < k; ++i)
        num_prod *= numer[i];
    for (idx_t i = 0; i < k; ++i)
        denom_prod *= denom[i];

    /* ----- Rounded division ----- */
    seq_t result = num_prod / denom_prod;
    seq_t rem = num_prod % denom_prod;
    if ((rem << 1) >= denom_prod)  /* round to nearest integer */
        result++;

    return result;
}
