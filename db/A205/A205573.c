/*
 * A205573  Array M read by antidiagonals in which successive rows evidently
 *          converge to A001405 (central binomial coefficients).
 * https://oeis.org/A205573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A205573
 *
 * The array M is defined by the bottom‑right entry of powers of the
 * tridiagonal matrix B_n (size n+1).  In terms of the sequence,
 * a(n) is the entry read by traversing M along antidiagonals
 * (rows are ordered by decreasing row index on each diagonal).
 *
 * 1. Find s = minimal integer such that (s+1)(s+2)/2 > n.
 * 2. Let offset = n - s*(s+1)/2.
 * 3. Set r = s - offset,  c = offset.
 * 4. a(n) = M(r,c) = (B_r)^c  (bottom‑right entry).
 *
 * The table of values M(r,c) for 0 ≤ r,c ≤ s is pre‑computed
 * once.  For r ≥ 1 the integers satisfy
 *   v[i] = v[i-1] + v[i+1]   (0<i<r)
 *   v[0] = v[1]
 *   v[r] = v[r-1] + v[r]     (self‑loop)
 * starting from v[r] = 1.  After each step we record the new
 * bottom‑right value as the sequence entry.
 *
 * Complexity:  O(s^2) pre‑computation (s ≈ √(2n)),  O(√n) per call.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Find s such that (s+1)(s+2)/2 > n */
    idx_t s = 0;
    while ( ((unsigned __int128)(s + 1) * (s + 2)) / 2 <= n )
        ++s;

    /* Offset inside diagonal s */
    idx_t tprev  = s * (s + 1) / 2;
    idx_t offset = n - tprev;
    idx_t r = s - offset;
    idx_t c = offset;

    /* Static table of M(r,c) values. Rebuild if needed. */
    static seq_t *table = NULL;   /* flattened (r * dim + c) */
    static int dim      = 0;      /* current table dimension */

    if ((int)s >= dim) {
        /* Grow table to dimension = s + 1 */
        if (table) free(table);
        dim = (int)s + 1;
        size_t total = (size_t)dim * (size_t)dim;
        table = (seq_t*)calloc(total, sizeof(seq_t));  /* zero‑init */

        /* Compute entire table up to current dimension */
        for (int rr = 0; rr < dim; ++rr) {
            if (rr == 0) {
                /* Row 0: all ones */
                for (int k = 0; k < dim; ++k)
                    table[rr * dim + k] = (seq_t)1;
                continue;
            }

            /* Temporary vectors for dynamic program */
            seq_t *v = (seq_t*)calloc(rr + 1, sizeof(seq_t));
            v[rr] = (seq_t)1;           /* start vector: only last element 1 */
            table[rr * dim + 0] = (seq_t)1; /* f[rr][0] */

            for (int step = 1; step < dim; ++step) {
                seq_t *newv = (seq_t*)calloc(rr + 1, sizeof(seq_t));
                for (int i = 0; i <= rr; ++i) {
                    seq_t val = 0;
                    if (i > 0)   val += v[i - 1];
                    if (i < rr)  val += v[i + 1];
                    if (i == rr) val += v[i];   /* self‑loop at last node */
                    newv[i] = val;
                }
                free(v);
                v = newv;
                table[rr * dim + step] = v[rr];   /* bottom‑right entry */
            }
            free(v);
        }
    }

    return table[r * dim + c];
}
