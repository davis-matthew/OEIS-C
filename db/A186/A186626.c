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
 * Validated: standard, pass, 29 terms through n = 197 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#pragma once
#include <stdint.h>

/* Count of ways to fill a 3‑digit row with digit 0..3 such that the row sums to s.
   Precomputed once. */
static void compute_cnt(seq_t *cnt) {
    for (int s = 0; s <= 9; ++s) cnt[s] = 0;
    for (int a = 0; a <= 3; ++a)
        for (int b = 0; b <= 3; ++b)
            for (int c = 0; c <= 3; ++c)
                ++cnt[a + b + c];
}

/* Computes f(L) as described above.  L >= 1. */
static seq_t f_of_len(idx_t L, const seq_t *cnt) {
    if (L == 0) return (seq_t)1;          /* empty sequence – one way */
    /* dp_last[s] = number of ways for current length, last sum = s. */
    seq_t dp_last[10];
    for (int s = 0; s <= 9; ++s) dp_last[s] = cnt[s];
    /* length 1 already computed. */
    for (idx_t len = 2; len <= L; ++len) {
        seq_t prefix[10];
        prefix[0] = dp_last[0];
        for (int s = 1; s <= 9; ++s) prefix[s] = prefix[s-1] + dp_last[s];
        seq_t dp_next[10];
        for (int t = 0; t <= 9; ++t)
            dp_next[t] = prefix[t] * cnt[t];
        for (int s = 0; s <= 9; ++s) dp_last[s] = dp_next[s];
    }
    seq_t total = 0;
    for (int s = 0; s <= 9; ++s) total += dp_last[s];
    return total;
}

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
