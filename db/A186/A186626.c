/*
 * A186626  Number of (n+2)X3 0..3 arrays with 3X3 subblock sums nondecreasing
 *          rightwards and downwards.
 * https://oeis.org/A186626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{r=0}^{2} f(L_r) , where L_r = floor((n+2-1 - r)/3)+1
 *            (number of rows in residue class r), cnt[s] = number of 3‑digit
 *            rows 0..3 with sum s (0 ≤ s ≤ 9), f(1) = Σ_s cnt[s], f(L+1) =
 *            Σ_t ( Σ_{s≤t} f_curr[s] ) * cnt[t]. The answer is the product of
 *            the three f(L_r)
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 10, u128 n <= 26,
 *            big any n
 * Helpers:   compute_cnt, f_of_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#pragma once
#include <stdint.h>
#include "../utils/compute_cnt.h"
#include "../utils/f_of_len.h"

/* Main sequence generator. */
seq_t a(idx_t n) {
    /* Compute the three class lengths. */
    idx_t total_rows = n + (idx_t)2;       /* (n+2) rows in total */
    idx_t len_mod[3];
    for (int r = 0; r < 3; ++r) {
        if (total_rows <= (idx_t)r) len_mod[r] = 0;
        else
            len_mod[r] = ((total_rows - (idx_t)1 - (idx_t)r) / (idx_t)3) + (idx_t)1;
    }

    /* Precompute row‑sum multiplicities (once). */
    static seq_t cnt[10];
    static int cnt_initialized = 0;
    if (!cnt_initialized) {
        compute_cnt(cnt);
        cnt_initialized = 1;
    }

    /* Compute f(L) for each class and multiply. */
    seq_t result = (seq_t)1;
    for (int r = 0; r < 3; ++r) {
        seq_t f = f_of_len(len_mod[r], cnt);
        result *= f;
    }
    return result;
}
