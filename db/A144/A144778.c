/*
 * A144778  Square array T(n,k) read by antidiagonals upwards. Mobius function
 *          interleaved with k-1 zeros in each column. Every second value in
 *          the first row is either 1 or -1. Redheffer type matrix.
 * https://oeis.org/A144778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 96, i16 n <= 96, i32 n <= 96, i64 n <= 96, i128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A144778 */
#include <math.h>

static inline int mobius_uint64(uint64_t x)
{
    if (x == 1) return 1;
    int count = 0;
    if (x % 2 == 0) {
        x /= 2;
        count++;
    }
    if (x % 2 == 0) return 0;
    for (uint64_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0) {
            x /= p;
            if (x % p == 0) return 0;
            count++;
        }
    }
    if (x > 1) count++;          /* remaining prime factor */
    return (count & 1) ? -1 : 1;
}

seq_t a(idx_t n)
{
    /* Determine the antidiagonal (sum s = m + 1) that contains index n.
       m is the length of that diagonal; 0‑based indexing with m >= 1.
    */
    idx_t m;
    double d = sqrt((double)(8.0 * n + 1.0));
    m = (idx_t)((d - 1.0) / 2.0);
    while ((m * (m + 1)) / 2 < n) m++;
    /* Tprev = (m-1)*m/2  */
    idx_t Tprev = ((m - 1) * m) / 2;
    idx_t pos = n - Tprev;   /* 1‑based position within the diagonal, 1 <= pos <= m */
    idx_t row = m - pos + 1;                  /* n index of the row   */
    idx_t col = (m + 1) - row;                /* k index of the column */

    if (row == 1) {
        /* First row alternates +1,-1, ... (k starts at 1) */
        return (col & 1) ? (seq_t)1 : (seq_t)-1;
    } else {
        if (row % col == 0) {
            uint64_t r = row / col;
            int mu = mobius_uint64(r);
            return (seq_t)mu;
        } else {
            return (seq_t)0;
        }
    }
}
