/*
 * A211312  Square array of Delannoy numbers D(i,j) mod 3 (i >= 0, j >= 0)
 *          read by antidiagonals.
 * https://oeis.org/A211312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Σ_{k=0}^{n} binomial(k, j) * binomial(n-j, k) (mod 3)
 *            which equals D(i,j) mod 3 where (i,j) is the point read by
 *            antidiagonals
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 90 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t delannoy_mod3(idx_t i, idx_t j) {
    /* Compute D(i,j) mod 3 using the recurrence:  
       D(0,0)=1, D(i,0)=D(0,j)=1,
       D(i,j)=D(i-1,j)+D(i,j-1)+D(i-1,j-1)  (mod 3).                */
    /* We keep a fixed small table because i,j are at most about 158 for
       n <= 10000 (and even larger n grow only linearly). */
    static seq_t tab[200][200];
    for (idx_t r = 0; r <= i; ++r) {
        for (idx_t c = 0; c <= j; ++c) {
            if (r == 0 || c == 0)
                tab[r][c] = 1;
            else
                tab[r][c] = (tab[r-1][c] + tab[r][c-1] + tab[r-1][c-1]) % 3;
        }
    }
    return tab[i][j];
}

seq_t a(idx_t n) {
    /* Find the anti‑diagonal d so that n lies on it.  The total number
       of elements up to and including diagonal d is (d+1)(d+2)/2. */
    idx_t d = 0;
    for (;; ++d) {
        idx_t total = (d + 1) * (d + 2) / 2;
        if (n < total)
            break;
    }
    /* Position of n within this diagonal (0‑based), which equals i. */
    idx_t pos = n - d * (d + 1) / 2;
    idx_t i = pos;
    idx_t j = d - pos;
    return delannoy_mod3(i, j);
}
