/*
 * A067337  Triangle where T(n,k)=2*T(n,k-1)+C(n-1,k)-C(n-1,k-1) and n>=k>=0.
 * https://oeis.org/A067337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A067337 – triangle where   
    T(n,k) = 2*T(n,k-1) + C(n-1,k) – C(n-1,k-1)  (n ≥ k ≥ 0)
    The flattened columns of the triangle are the terms a(n). */

#include <stdint.h>   /* for uint64_t, if needed */

seq_t a(idx_t n) {
    /* Determine the row r of the flattened triangle that contains a(n). */
    idx_t r = 0;
    /* find smallest r such that (r+1)*(r+2)/2 > n  */
    while ((r + 1) * (r + 2) / 2 <= n) r++;

    /* index of first element of this row */
    idx_t base = r * (r + 1) / 2;
    /* position within the row */
    idx_t k = n - base;

    /* T(r,0) is always 1 */
    if (k == 0) return (seq_t)1;

    /* Compute T(r,k) iteratively using the recurrence. */
    seq_t t = (seq_t)1;          /* current value T(r, j-1) */
    seq_t binom_prev = (seq_t)1; /* C(r-1, j-1)  */

    for (idx_t j = 1; j <= k; ++j) {
        /* binom = C(r-1, j) = binom_prev * (r-1-(j-1)) / j  */
        seq_t numer = binom_prev * ((seq_t)(r - 1) - (seq_t)(j - 1));
        seq_t denom = (seq_t)j;
        seq_t binom = numer / denom;                 /* exact division */

        seq_t diff = binom - binom_prev;             /* C(r-1,j) - C(r-1,j-1) */
        t = (t << 1) + diff;                         /* 2*t + diff */

        binom_prev = binom;                          /* prepare for next j */
    }
    return t;
}
