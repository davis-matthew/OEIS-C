/*
 * A182898  Triangle read by rows: T(n,k) is the number of weighted lattice
 *          paths in L_n having k returns to the horizontal axis (both from
 *          above and below). The members of L_n are paths of weight n that
 *          start at (0,0) , end on the horizontal axis and whose steps are of
 *          the following four kinds: an (1,0)-ste...
 * https://oeis.org/A182898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n-th term of the triangle read by rows, where the
 *            triangle T(n,k) counts weighted lattice paths of weight n having
 *            k returns to the horizontal axis. The generating function for
 *            T(n,k) is G(t,z) = 1 / (1 - z - z^2 - 2 t z^3 c(z)), with c(z)
 *            satisfying c(z) = 1 + z c(z) + z^2 c(z) + z^3...
 * Range:     u8 n <= 15, u16 n <= 45, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static state variables, initialized on first call */
    static int initialized = 0;
    static idx_t currentWeight = 0;            /* last weight processed */
    static seq_t *c = NULL;   /* coefficients of c(z), c[0] = 1 */
    static size_t c_cap = 0, c_used = 0;
    static seq_t **dpRows = NULL;   /* dpRows[w] holds T(w,k) for k=0..w/3 */
    static size_t dp_rows_cap = 0, dp_rows_used = 0;
    static seq_t *seqVals = NULL;   /* flattened sequence values */
    static size_t seq_cap = 0, seq_used = 0;

    /* helper to ensure capacity of a raw pointer */
    if (!initialized) {
        /* initial c[0] = 1 */
        if (c_cap < 1) {
            c_cap = 8;
            c = (seq_t *)malloc(c_cap * sizeof(seq_t));
        }
        c[0] = 1;
        c_used = 1;

        /* initial dpRows[0] with T(0,0) = 1 */
        if (dp_rows_cap < 1) {
            dp_rows_cap = 8;
            dpRows = (seq_t **)malloc(dp_rows_cap * sizeof(seq_t *));
        }
        seq_t *row0 = (seq_t *)calloc(1, sizeof(seq_t));
        row0[0] = 1;
        dpRows[0] = row0;
        dp_rows_used = 1;

        /* initial seqVals with first term T(0,0) */
        if (seq_cap < 1) {
            seq_cap = 16;
            seqVals = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        }
        seqVals[0] = 1;
        seq_used = 1;

        currentWeight = 0;
        initialized = 1;
    }

    /* expand table until we have at least n+1 values */
    while (seq_used <= n) {
        /* move to next weight */
        currentWeight++;
        size_t w = (size_t)currentWeight;
        /* compute c[w] if needed */
        if (c_used <= w) {
            if (c_cap < w + 1) {
                while (c_cap < w + 1) c_cap *= 2;
                c = (seq_t *)realloc(c, c_cap * sizeof(seq_t));
            }
            if (w == 1) c[1] = 1;
            else if (w == 2) c[2] = 2;
            else {
                seq_t sum = c[w - 1] + c[w - 2];
                for (size_t i = 0; i <= w - 3; ++i) {
                    seq_t prod = c[i] * c[w - 3 - i];
                    sum += prod;
                }
                c[w] = sum;
            }
            c_used = w + 1;
        }

        /* compute new dpRows[w] for T(w,k) */
        size_t maxK = w / 3;
        if (dp_rows_cap <= currentWeight) {
            while (dp_rows_cap <= currentWeight) dp_rows_cap *= 2;
            dpRows = (seq_t **)realloc(dpRows, dp_rows_cap * sizeof(seq_t *));
        }
        seq_t *row = (seq_t *)calloc(maxK + 1, sizeof(seq_t));
        for (size_t k = 0; k <= maxK; ++k) {
            seq_t val = 0;

            /* horizontal steps of weight 1 */
            if (w >= 1) {
                size_t len1 = ((w - 1) / 3) + 1;
                if (k < len1) val += dpRows[w - 1][k];
            }
            /* horizontal steps of weight 2 */
            if (w >= 2) {
                size_t len2 = ((w - 2) / 3) + 1;
                if (k < len2) val += dpRows[w - 2][k];
            }
            /* return blocks (weight >=3) */
            if (k >= 1) {
                for (size_t bw = 3; bw <= w; ++bw) {
                    size_t inner_w = w - bw;
                    size_t len_inner = (inner_w / 3) + 1;
                    if (k - 1 < len_inner) {
                        seq_t coeff = c[bw - 3];
                        coeff += coeff;   /* multiply by 2 */
                        val += coeff * dpRows[inner_w][k - 1];
                    }
                }
            }
            row[k] = val;
        }
        dpRows[w] = row;
        dp_rows_used = currentWeight + 1;

        /* append row entries to the flattened sequence */
        for (size_t k = 0; k <= maxK; ++k) {
            if (seq_cap <= seq_used) {
                while (seq_cap <= seq_used) seq_cap *= 2;
                seqVals = (seq_t *)realloc(seqVals, seq_cap * sizeof(seq_t));
            }
            seqVals[seq_used++] = row[k];
        }
    }

    return seqVals[n];
}
