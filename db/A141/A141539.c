/*
 * A141539  Square array A(n,k) of numbers of length n binary words with at
 *          least k "0" between any two "1" digits (n,k >= 0), read by
 *          antidiagonals.
 * https://oeis.org/A141539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * A(n,k) = 2^n if k = 0 * A(n,k) = n + 1 if n <= k * A(n,k) =
 *            A(n-1,k) + A(n-k-1,k) otherwise * a(n) = A(i, k) where i = n -
 *            T(m) * k = m - i, T(m) = m*(m+1)/2, * m = ⌊(√(8n+1)-1)/2⌋
 *            (antidiagonal index)
 * Range:     u8 n <= 43, u16 n <= 151, u32 n <= 559, u64 n <= 2143,
 *            u128 n <= 8383, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- utilities ---------- */

static inline idx_t isqrt_u64(idx_t x) {
    idx_t lo = 0, hi = (idx_t)1 << 32;          /* 2^32 > sqrt(UINT64_MAX) */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((unsigned __int128)mid * mid <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static inline seq_t pow2(idx_t e) {
    seq_t r = 1;
    for (idx_t i = 0; i < e; ++i)
        r <<= 1;                       /* works for all seq_t types */
    return r;
}

/* ---------- table storage ---------- */

static seq_t *A_table = NULL;   /* row-major: index = n * dim + k */
static idx_t A_dim   = 0;       /* current dimension of table (size = dim × dim) */

/* ensure table has dimension >= d (rows and columns) */
static void ensure_table(idx_t d) {
    if (d <= A_dim)
        return;

    size_t new_size = (size_t)d * (size_t)d;
    seq_t *new_tbl = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_tbl) abort();       /* allocation failure */

    /* compute values for the new table
       note: we recompute all entries even the old ones; size is small   */
    for (idx_t n = 0; n < d; ++n) {
        for (idx_t k = 0; k < d; ++k) {
            seq_t val;
            if (k == 0) {
                val = pow2(n);                     /* k = 0  -> 2^n */
            } else if (n <= k) {
                val = (seq_t)(n + 1);              /* n <= k */
            } else {
                /* n > k > 0 */
                val = new_tbl[(n - 1) * d + k]               /* A(n-1,k) */
                      + new_tbl[(n - k - 1) * d + k];         /* A(n-k-1,k) */
            }
            new_tbl[n * d + k] = val;
        }
    }

    /* free old table if any, replace with new one */
    if (A_table)
        free(A_table);
    A_table = new_tbl;
    A_dim   = d;
}

/* ---------- main sequence function ---------- */

seq_t a(idx_t n) {
    /* find diagonal index m so that T(m) <= n < T(m+1) */
    idx_t t  = 8 * n + 1;                 /* 8n+1 fits in idx_t for n up to 10k */
    idx_t sq = isqrt_u64(t);               /* floor sqrt(8n+1) */
    idx_t m  = (sq - 1) / 2;               /* diagonal number */
    idx_t T_m = m * (m + 1) / 2;           /* starting index of diagonal m */
    idx_t offset = n - T_m;                /* position along the diagonal (0..m) */
    idx_t k = m - offset;                  /* corresponding k */

    /* ensure table contains entries for indices up to m */
    ensure_table(m + 1);

    return A_table[offset * A_dim + k];
}
