/*
 * A080061  Triangle of coefficients of polynomials P(n; x) = Permanent(M),
 *          where M=[m(i,j)] is n X n matrix defined by m(i,j)=x if 0<=i-j<=2
 *          else m(i,j)=1.
 * https://oeis.org/A080061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient of x^k in the permanent of the n × n matrix M
 *            where m(i,j)=x if 0≤i-j≤2, otherwise 1. The coefficient is given
 *            by a(n,k) = Σ_{j=k}^{n} (-1)^{j-k} * C(j,k) * r_j * (n-j)! where
 *            r_j is the j‑th rook number of the board of the special (band)
 *            elements. The index n of this function...
 * Range:     u8 n <= 28, u16 n <= 46, u32 n <= 105, u64 n <= 209,
 *            u128 n <= 209, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* -------------  basic helpers  ---------------- */

#include <stddef.h>      /* size_t */
#include <stdint.h>      /* uint64_t */
#include <string.h>      /* memset */

/* Include the utilities that are available in the build environment.  */
#include "../utils/binomial.h"   /* binomial(idx_t n, idx_t k) -> seq_t */

/* -------------  rook number calculation  ---------------- */

static seq_t *alloc_seq_array(size_t n) {
    seq_t *p = (seq_t*)malloc(n * sizeof(seq_t));
    if (!p) exit(1);
    memset(p, 0, n * sizeof(seq_t));
    return p;
}

/* Compute rook numbers r[0..n] for the n×n band board.
   r[j] = number of ways to place j non‑attacking rooks on the
   special cells { (i,j) | i 0≤i<n, max(0,i-2) ≤ j ≤ i }.
   Complexity: O(n²).  */
static seq_t *rook_numbers(idx_t n) {
    size_t N = (size_t)n;                 /* actual size for C indices */
    /* DP arrays: mask 0..7, k 0..i */
    seq_t **cur = (seq_t**)malloc(8 * sizeof(seq_t*));
    seq_t **nxt = (seq_t**)malloc(8 * sizeof(seq_t*));
    for (int m = 0; m < 8; ++m) {
        cur[m] = alloc_seq_array(N+1);
        nxt[m] = alloc_seq_array(N+1);
    }
    cur[0][0] = 1;                         /* start state: no columns used */

    for (size_t i = 0; i < N; ++i) {        /* process row i */
        /* clear nxt */
        for (int m = 0; m < 8; ++m)
            memset(nxt[m], 0, (N+1)*sizeof(seq_t));

        for (int mask = 0; mask < 8; ++mask) {
            for (size_t k = 0; k <= i; ++k) {
                seq_t val = cur[mask][k];
                if (!val) continue;

                /* 1. Do not place a rook in this row */
                int newmask = (mask >> 1);
                nxt[newmask][k] += val;

                /* 2. Try to place a rook in any free special column */
                for (int bit = 0; bit < 3; ++bit) {
                    /* column index for this bit */
                    if ((bit == 0 && i < 2) ||
                        (bit == 1 && i < 1)) /* column does not exist */
                        continue;

                    if (mask & (1 << bit)) continue; /* already used */

                    int nm = ((mask | (1 << bit)) >> 1);
                    nxt[nm][k + 1] += val;
                }
            }
        }
        /* swap cur and nxt */
        for (int m = 0; m < 8; ++m) {
            seq_t *tmp = cur[m];
            cur[m]  = nxt[m];
            nxt[m]  = tmp;
        }
    }

    /* accumulate rook numbers */
    seq_t *rook = alloc_seq_array(N+1);
    for (size_t k = 0; k <= N; ++k) {
        seq_t tot = 0;
        for (int mask = 0; mask < 8; ++mask)
            tot += cur[mask][k];
        rook[k] = tot;
    }

    /* free DP arrays */
    for (int m = 0; m < 8; ++m) {
        free(cur[m]); free(nxt[m]);
    }
    free(cur); free(nxt);
    return rook;
}

/* -------------  factorial for small n  ---------------- */

static seq_t *fact_array(idx_t n) {
    size_t N = (size_t)n;
    seq_t *fact = alloc_seq_array(N+1);
    fact[0] = 1;
    for (size_t i = 1; i <= N; ++i)
        fact[i] = fact[i-1] * (seq_t)i;
    return fact;
}

/* -------------  main sequence function  ---------------- */

seq_t a(idx_t n_index) {
    /* determine row r and position k within that row */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n_index) ++r;

    idx_t start = r * (r + 1) / 2;
    idx_t k     = n_index - start;          /* 0‑based position in row r */

    /* compute rook numbers for board of size r */
    seq_t *rook = rook_numbers(r);
    seq_t *fact = fact_array(r);

    /* compute coefficient a(r,k) via inclusion–exclusion */
    seq_t sum_pos = 0;
    seq_t sum_neg = 0;
    for (idx_t j = k; j <= r; ++j) {
        seq_t comb  = binomial(j, k);           /* C(j,k) */
        seq_t term  = comb * rook[j] * fact[r - j];
        if ((j - k) & 1)   /* odd -> subtract */
            sum_neg += term;
        else
            sum_pos += term;
    }

    /* final result: sum_pos >= sum_neg always for this sequence */
    seq_t result = sum_pos - sum_neg;

    /* free temporary arrays */
    free(rook);
    free(fact);
    return result;
}
