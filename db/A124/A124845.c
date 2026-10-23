/*
 * A124845  Triangle read by rows: T(n,k) = (3 - (-1)^k)*binomial(n,k)/2 (0 <=
 *          k <= n).
 * https://oeis.org/A124845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ((3-(-1)^k)/2) * binomial(r,k) (0 ≤ k ≤ r) * where r =
 *            floor((sqrt(8*n+1)-1)/2) and k = n - r*(r+1)/2
 * Range:     u8 n <= 59, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(sqrt(x)) for 0 <= x < 2^64.
   The routine uses a bit‑by‑bit construction that works with 128‑bit
   intermediate products to avoid overflow. */
static inline idx_t floor_sqrt_u64(idx_t x)
{
    idx_t r = 0;
    for (int i = 63; i >= 0; --i) {
        idx_t bit   = 1ULL << i;
        idx_t cand  = r | bit;
        if ((unsigned __int128)cand * cand <= x)
            r = cand;
    }
    return r;
}

/* Compute binomial(n,k) multiplicatively in seq_t.
   Uses symmetry to minimise the number of multiplications. */
static seq_t binom_nk(idx_t n, idx_t k)
{
    if (k > n)              return 0;
    if (k > n - k)          k = n - k;   /* use the smaller side */

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* res = res * (n - k + i) / i  – the division is exact */
        res = (res * (seq_t)(n - k + i)) / (seq_t)i;
    }
    return res;
}

/* External linkage as required by the build system. */
seq_t a(idx_t n)
{
    /* Find row index r such that start(r) ≤ n < start(r+1) */
    idx_t d = 1 + 8 * n;                     /* discriminant for quadratic */
    idx_t s = floor_sqrt_u64(d);
    idx_t r = (s - 1) / 2;

    /* Compute column index k within row r */
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    /* T(r,k) = (3 - (-1)^k)/2 * binomial(r,k)  →  binom * (k odd? 2 : 1) */
    seq_t val = binom_nk(r, k);
    if (k & 1U)                            /* multiply by 2 when k is odd */
        val = val * 2;

    return val;
}
