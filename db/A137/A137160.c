/*
 * A137160  G.f.: 1 = Sum_{n>=0} a(n)*x^n*(1-x)/(1+x)^(2^n).
 * https://oeis.org/A137160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1. For n ≥ 1, a(n) = Σ_{k=0}^{n-1} a(k) * (-1)^{n-k+1} *
 *            ( B_{k,n-k} + B_{k,n-k-1} ), where B_{k,m} = binom(2^k + m - 1,
 *            m) (positive integer). The sums are carried out using unsigned
 *            arithmetic by splitting the terms into positive and negative
 *            contributions. The arrays a, 2^k, the magnitu...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Static storage for memoisation */
    static seq_t *val      = NULL;   /* a[0..max]          */
    static seq_t *pow2_tbl = NULL;   /* 2^i  for i          */
    static seq_t *Bmag     = NULL;   /* B_{i,k} magnitude   */
    static idx_t *k_cur    = NULL;   /* current k for each i*/
    static idx_t  cur_max   = 0;     /* last computed index */
    static idx_t  size     = 0;      /* allocated size     */

    /* Extend storage if necessary */
    if (n + 1 > size) {
        idx_t new_size = n + 1;
        seq_t *new_val  = (seq_t*)calloc(new_size, sizeof(seq_t));
        seq_t *new_pow2 = (seq_t*)calloc(new_size, sizeof(seq_t));
        seq_t *new_Bmag = (seq_t*)calloc(new_size, sizeof(seq_t));
        idx_t *new_k_cur= (idx_t*)calloc(new_size, sizeof(idx_t));

        /* copy previous data */
        if (val)   { memcpy(new_val,   val,   size * sizeof(seq_t)); free(val); }
        if (pow2_tbl) { memcpy(new_pow2, pow2_tbl, size * sizeof(seq_t)); free(pow2_tbl); }
        if (Bmag)  { memcpy(new_Bmag, Bmag,  size * sizeof(seq_t)); free(Bmag); }
        if (k_cur) { memcpy(new_k_cur, k_cur, size * sizeof(idx_t)); free(k_cur); }

        /* initialise new entries (k = 0 → Bmag = 1) */
        for (idx_t i = size; i < new_size; ++i) {
            new_val[i]   = 0;
            new_pow2[i]  = pow2(i);   /* 2^i */
            new_Bmag[i]  = 1;         /* B at k=0 */
            new_k_cur[i] = 0;         /* k starts at 0 */
        }
        if (size == 0) new_val[0] = 1;  /* a(0) = 1 */

        /* install new arrays */
        val      = new_val;
        pow2_tbl = new_pow2;
        Bmag     = new_Bmag;
        k_cur    = new_k_cur;
        size     = new_size;
    }

    /* Compute missing a[ cur_max+1 .. n ] */
    for (idx_t N = cur_max + 1; N <= n; ++N) {
        seq_t pos_sum = 0;
        seq_t neg_sum = 0;

        for (idx_t i = 0; i < N; ++i) {
            idx_t k_old = k_cur[i];
            seq_t B_old = Bmag[i];

            /* B_new = B_old * (2^i + k_old) / (k_old+1) */
            seq_t mult   = B_old * (pow2_tbl[i] + k_old);
            seq_t B_new  = mult / (k_old + 1);
            seq_t A      = B_new + B_old;          /* B_new + B_old */
            seq_t prod   = val[i] * A;            /* a[i] * A     */

            /* sign: (-1)^{N-i+1}  → positive when (N-i) is odd */
            if ( ((N - i) & 1) == 1 )
                pos_sum += prod;                  /* positive contribution */
            else
                neg_sum += prod;                  /* negative contribution */

            /* update B and current k */
            Bmag[i] = B_new;
            k_cur[i] = k_old + 1;
        }

        /* a[N] = pos_sum - neg_sum   (pos_sum ≥ neg_sum) */
        val[N] = pos_sum - neg_sum;
    }

    cur_max = n;
    return val[n];
}
