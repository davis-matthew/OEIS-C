/*
 * A068009  Square array T(m,n) with m (row) >= 1 and n (column) >= 0 read by
 *          antidiagonals: number of subsets of {1,2,3,...n} that sum to 0 mod
 *          m (including the empty set, whose sum is 0).
 * https://oeis.org/A068009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, tabl
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 135, u32 n <= 527, u64 n <= 2079,
 *            u128 n <= 8255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A068009: T(m,n) read by antidiagonals.
   a(n) = number of subsets of {1,2,…,k} whose sum ≡ 0 (mod m),
   where (m,k) is obtained from n by unfolding the antidiagonal
   indexing used in the OEIS table.  This is computed by a
   DP over the residue classes modulo m. */

#include <stdint.h>
#include <stddef.h>

/* Maximum width needed for dp tables.
   For n up to 10 000 the largest m is < 150, so 256 is safe. */
#define MAX_M 256

/* Static DP buffers: two rows of length MAX_M. */
static seq_t dp0[MAX_M];
static seq_t dp1[MAX_M];
static seq_t *dp[2] = { dp0, dp1 };

/* ------------------------------------------------------------------
   Computes T(m,n): number of subsets of {1,…,n} with sum ≡ 0 (mod m).
   ------------------------------------------------------------------ */
static seq_t compute_T(idx_t m, idx_t n)
{
    size_t cap  = (size_t)m;      /* number of residues 0 … m-1   */
    seq_t *cur  = dp[0];
    seq_t *next = dp[1];

    /* initialize cur to zero; cur[0] = 1 represents the empty set */
    for (size_t i = 0; i < cap; ++i)
        cur[i] = 0;
    cur[0] = 1;

    for (idx_t x = 1; x <= n; ++x) {
        size_t r = (size_t)(x % m);
        for (size_t y = 0; y < cap; ++y) {
            size_t idx = (y + cap - r) % cap;
            next[y] = cur[y] + cur[idx];
        }
        /* swap cur and next */
        seq_t *tmp  = cur;
        cur  = next;
        next = tmp;
    }
    return cur[0];
}

/* ------------------------------------------------------------------
   Unravels index n into (m,k) according to the antidiagonal ordering.
   ------------------------------------------------------------------ */
static void unravel(idx_t n, idx_t *m_out, idx_t *k_out)
{
    /* find smallest s such that s*(s+1)/2 > n */
    idx_t s   = 1;
    idx_t sum = 1;            /* 1*2/2 */
    while (sum <= n) {
        ++s;
        sum = s * (s + 1) / 2;
    }
    idx_t prev_sum = (s - 1) * s / 2;
    idx_t offset   = n - prev_sum;      /* 0‑based offset on the diagonal   */
    *m_out = offset + 1;                /* rows are 1‑based                 */
    *k_out = s - *m_out;                /* columns start at 0                */
}

/* ------------------------------------------------------------------
   Public sequence function used by the OEIS engine.
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    idx_t m, k;
    unravel(n, &m, &k);
    return compute_T(m, k);
}
