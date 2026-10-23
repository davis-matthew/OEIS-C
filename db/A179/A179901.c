/*
 * A179901  Triangle read by rows, antidiagonals of an array generated from
 *          (1, r, r, r, ...) convolved with (1, 0, r, r, r, ...).
 * https://oeis.org/A179901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A179901: Triangle read by rows, antidiagonals of an array whose
 * r‑th row is the convolution of (1,r,r,…) with (1,0,r,r,…).
 *
 * For r,c ≥ 1 the array element is
 *   A[r][c] = 1                , if c==1
 *   A[r][c] = r                , if c==2
 *   A[r][c] = 2*r + (c-3)*r²   , if c≥3
 *
 * The sequence a(n) is formed by taking each antidiagonal of this array,
 * reversing it, and listing the elements row by row. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The first few terms are 1,1,1,1,… */
    if (n <= 3) return (seq_t)1;

    /* Find the diagonal length L such that L*(L+1)/2 >= n.
       The diagonal has length L and contains the terms n in the
       range  (L-1)*L/2 + 1  …  L*(L+1)/2. */
    idx_t L = 1;
    seq_t tri;
    for (;; ++L) {
        tri = (seq_t)L * ((seq_t)L + 1) / 2;
        if (tri >= n) break;
    }

    /* Start index of this diagonal and position within it. */
    idx_t start = (L - 1) * L / 2 + 1;
    idx_t k = n - start;          /* 0‑based offset inside the diagonal */

    /* Reversed diagonal: r = L - k,   c = k + 1. */
    idx_t r = L - k;
    idx_t c = k + 1;

    seq_t rr = (seq_t)r;
    seq_t cc = (seq_t)c;

    if (cc == 1) return (seq_t)1;
    if (cc == 2) return rr;

    /* For c ≥ 3: A[r][c] = 2*r + (c-3)*r^2. */
    return 2 * rr + (cc - 3) * (rr * rr);
}
