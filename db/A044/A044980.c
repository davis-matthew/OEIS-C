/*
 * A044980  Numbers whose base-3 representation contains exactly three 0's and
 *          no 1's.
 * https://oeis.org/A044980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers whose base‑3 representation contains exactly three 0's
 *            and no 1's. The term a(n) (offset = 1) is the n‑th such number
 *            in increasing order
 * Range:     u8 n <= 5, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-01)
 * Helpers:   binomial_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial_idx.h"

static inline void choose_desc_rank(idx_t rank, idx_t total, idx_t k, idx_t *pos) {
    /* Convert a 0‑based rank into a descending lexicographic combination of `k`
       positions chosen from 0..total-1.  The resulting array `pos` contains the
       selected positions in descending order. */
    idx_t r = rank;
    for (idx_t i = 0; i < k; ++i) {
        idx_t start = (i == 0 ? total - 1 : pos[i - 1] - 1);
        for (idx_t x = start;; --x) {
            idx_t cnt = binomial_idx(x, k - i - 1);
            if (r >= cnt) {
                r -= cnt;
            } else {
                pos[i] = x;
                break;
            }
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for offset 1, but defensive */

    /* 1. Find the length L of the base‑3 representation for the n‑th term.
       For a given L the number of qualifying numbers is C(L‑1,3); the
       cumulative count up to length L equals C(L,4). */
    idx_t L = 4;
    while (binomial_idx(L, 4) < n) ++L;

    /* 2. Determine the rank of n within numbers of length L. */
    idx_t prev = binomial_idx(L - 1, 4);   /* numbers of lengths < L */
    idx_t rank = n - prev - 1;       /* 0‑based rank within length L */

    /* 3. Find the zero positions (descending lexicographic). */
    idx_t zero_pos[3];
    choose_desc_rank(rank, L - 1, 3, zero_pos);

    /* 4. Compute the value by adding 2 * 3^i for all non‑zero positions. */
    seq_t value = 0;
    seq_t pow3  = 1;        /* 3^0 */
    for (idx_t i = 0; i < L; ++i) {
        if (i == zero_pos[0] || i == zero_pos[1] || i == zero_pos[2]) {
            /* digit is 0 – skip */
        } else {
            value += (seq_t)2 * pow3;
        }
        pow3 *= (seq_t)3;
    }
    return value;
}
