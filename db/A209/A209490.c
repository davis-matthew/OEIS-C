/*
 * A209490  Table T(d,n), read by antidiagonals, gives the number of subsets
 *          of length n containing an arithmetic progression of length 3 with
 *          distance d.
 * https://oeis.org/A209490
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(d,n0) read by antidiagonals from Table T(d,n) where
 *            T(d,n)=2^n - prod_{i=0}^{d-1} Tri(floor((n+i)/d)+2) (Tri(0)=0,
 *            Tri(1)=1, Tri(2)=1)
 * Range:     u8 n <= 22, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 68 (2026-11-03)
 * Helpers:   mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---- auxiliary utilities ------------------------------------------------- */
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <assert.h>
/* seq_t and idx_t are defined by the build. */
#include "../utils/pow2.h"
#include "../utils/mem.h"

/* ---- local Tribonacci table (short, recomputed on each call) -------------- */
static void compute_tribonacci(seq_t *tri, size_t max_index)
{
    /* tri[0] = 0, tri[1] = 1, tri[2] = 1 */
    assert(max_index >= 2);
    tri[0] = (seq_t)0;
    tri[1] = (seq_t)1;
    tri[2] = (seq_t)1;
    for (size_t i = 3; i <= max_index; ++i)
        tri[i] = tri[i-1] + tri[i-2] + tri[i-3];
}

/* ---- map global index to (d,n0) ------------------------------------------ */
static void index_to_dn(idx_t idx, idx_t *out_d, idx_t *out_n)
{
    /* idx >= 3 is guaranteed when this function is called, but we guard */
    if (idx < 3) {
        *out_d = 0;
        *out_n = 0;
        return;
    }

    /* zero‑based offset inside the antidiagonal ordering */
    idx_t off = idx - (idx_t)3;

    /* Find minimal m such that m*(m+1)/2 > off   (m = S-3) */
    long double disc = sqrtl((long double)8.0L * (long double)off + 1.0L);
    idx_t m = (idx_t)((disc - 1.0L) / 2.0L);

    /* Adjust upward while the triangular number is not strictly larger */
    while (((__uint128_t)m * (m + (idx_t)1)) / (idx_t)2 <= off) ++m;

    idx_t S = m + (idx_t)3;                     /* sum d+n */
    idx_t cum_prev = ((idx_t)(m - (idx_t)1) * m) / (idx_t)2;   /* (m-1)*m/2 */
    idx_t pos_in_diag = off - cum_prev;          /* 0‑based within diagonal */

    /* Within diagonal S, d descends from m to 1 */
    idx_t d = m - pos_in_diag;
    idx_t n0 = S - d;

    *out_d = d;
    *out_n = n0;
}

/* ---- main sequence function ---------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < (idx_t)3) return (seq_t)0;   /* behaviour below offset undefined */

    idx_t d, n0;
    index_to_dn(n, &d, &n0);

    /* Maximum floor((n0 + i)/d) needed in the product */
    idx_t max_floor = (n0 + d - (idx_t)1) / d;
    size_t trib_needed = (size_t)(max_floor + (idx_t)2);   /* indices up to this */

    /* Build small tribonacci table */
    seq_t *trib = mem_new(seq_t, (trib_needed + 1));
    if (!trib) abort();   /* allocation failure */
    compute_tribonacci(trib, trib_needed);

    /* Compute product of tribonacci values */
    seq_t prod = (seq_t)1;
    for (idx_t i = 0; i < d; ++i) {
        idx_t floor_val = (n0 + i) / d;
        prod *= trib[(size_t)floor_val + 2];
    }

    mem_free(trib);

    seq_t pow2_val = pow2(n0);
    if (prod > pow2_val) return (seq_t)0;   /* safety, should not happen */
    return pow2_val - prod;
}
