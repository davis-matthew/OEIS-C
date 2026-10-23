/*
 * A167194  Triangle read by rows. A130713 in the columns.
 * https://oeis.org/A167194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A167194 (triangle read by rows).
   The triangle has rows:
      [1]
      [2,1]
      [1,2,1]
      [0,1,2,1]
      [0,0,1,2,1]
      ...
   Each row n contains the non‑zero entries of binomial(2, n-k) for k=1..n.
   The nth term is obtained by determining its row r and column k. */

#include <math.h>
#include <stdint.h>     /* for uint64_t */

seq_t a(idx_t n)
{
    /* Find r such that r(r+1)/2 >= n  (row index, 1‑based). */
    idx_t r;
    /* Initial approximation using double/long double. */
    long double x = sqrtl(8.0L * (long double)n + 1.0L);
    r = (idx_t)((x - 1.0L) / 2.0L);

    /* Adjust up if necessary. */
    __uint128_t tri = (__uint128_t)r * (r + 1) / 2;
    while (tri < n) {
        ++r;
        tri = (__uint128_t)r * (r + 1) / 2;
    }
    /* Adjust down if necessary. */
    while (r > 1) {
        idx_t prev = r - 1;
        __uint128_t prev_tri = (__uint128_t)prev * (prev + 1) / 2;
        if (prev_tri >= n) {
            r = prev;
            tri = prev_tri;
        } else {
            break;
        }
    }

    /* Position k within the row (1‑based). */
    __uint128_t start = (__uint128_t)(r - 1) * r / 2;   /* number of terms before row r */
    idx_t k = n - (idx_t)start;

    /* Compute m = r - k;  only m = 0,1,2 produce non‑zero values. */
    idx_t m = r - k;
    seq_t val;
    if (m == 0)            val = (seq_t)1;   /* binom(2,0) */
    else if (m == 1)       val = (seq_t)2;   /* binom(2,1) */
    else if (m == 2)       val = (seq_t)1;   /* binom(2,2) */
    else                   val = (seq_t)0;   /* binom(2,m>2) = 0 */

    return val;
}
