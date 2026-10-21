/*
 * A204016  Symmetric matrix based on f(i,j) = max(j mod i, i mod j), by
 *          antidiagonals.
 * https://oeis.org/A204016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth element of the matrix M(i,j)=max(j % i , i % j)
   read in antidiagonal order starting from (1,1). */
#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* n is >= 1; we use 1‑based indexing as in OEIS */
    /* Find smallest t such that t*(t+1)/2 >= n */
    /* t corresponds to the sum s-1 of the indices (i + j). */
    long double d = sqrtl(8.0L * (long double)n + 1.0L);
    idx_t t = (idx_t)((d - 1.0L) / 2.0L);          /* initial estimate */

    /* Adjust the estimate up or down to satisfy the inequality */
    while (((unsigned __int128) t * (t + 1)) / 2 < n) t++;
    while (t > 0 && (((unsigned __int128)(t - 1) * t) / 2) >= n) t--;

    /* s = t + 1  (the sum of i + j) */
    idx_t cumulative_prev = ((unsigned __int128)(t - 1) * t) / 2;
    idx_t offset = n - cumulative_prev - 1;      /* 0‑based position in the antidiagonal */

    idx_t i = offset + 1;
    idx_t j = t - offset;                        /* because s-1 == t */

    /* Compute max(j % i , i % j) */
    seq_t i_seq = (seq_t) i;
    seq_t j_seq = (seq_t) j;
    seq_t mod1 = j_seq % i_seq;
    seq_t mod2 = i_seq % j_seq;
    return (mod1 > mod2) ? mod1 : mod2;
}
