/*
 * A117207  Number triangle read by rows: T(n,k) = Sum_{j=0..n-k}
 *          C(n+j,j+k)*C(n-j,k).
 * https://oeis.org/A117207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the row‑wise reading of the triangle T(n,k) =
 *            ∑_{j=0..n-k} C(n+j, j+k) * C(n-j, k)
 * Range:     u8 n <= 14, u16 n <= 44, u32 n <= 152, u64 n <= 594,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses multiplicative binomial computation with
   128‑bit integers to avoid overflow. */

#include <stdint.h>
#include <math.h>

/* Compute binomial coefficient in unsigned 128 bits.  */
static inline unsigned __int128 binom128(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */
    unsigned __int128 res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;   /* exact division each step */
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Determine row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t r;
    long double t = sqrtl(8.0L * (long double)n + 1.0L);
    r = (idx_t)((t - 1.0L) / 2.0L);
    /* Correct possible rounding errors */
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n) ++r;
    while ((unsigned __int128)r * (r + 1) / 2 > n) --r;

    idx_t row_start = ((unsigned __int128)r * (r + 1)) / 2;
    idx_t k = n - row_start;

    /* Compute T(r,k) = Σ_{j=0..r-k} C(r+j, j+k) * C(r-j, k) */
    unsigned __int128 sum = 0;
    for (idx_t j = 0; j <= r - k; ++j) {
        unsigned __int128 a1 = binom128(r + j, j + k);
        unsigned __int128 a2 = binom128(r - j, k);
        sum += a1 * a2;
    }

    return (seq_t)sum;
}
