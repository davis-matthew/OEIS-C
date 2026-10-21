/*
 * A235791  Irregular triangle read by rows: T(n,k), n >= 1, k >= 1, in which
 *          column k lists k copies of every positive integer in nondecreasing
 *          order, and the first element of column k is in row k(k+1)/2.
 * https://oeis.org/A235791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r,k) where the triangle T(n,k) is read by rows. For 1 ≤
 *            n, 1 ≤ k ≤ floor((sqrt(8n+1)-1)/2), T(n,k) = ceil((n+1)/k -
 *            (k+1)/2). The function finds the row r and column k that contain
 *            the nth element and evaluates the formula using integer
 *            arithmetic only
 * Range:     u8 n <= 1600, u16 n <= 1600, u32 n <= 1600, u64 n <= 1600,
 *            u128 n <= 1600, big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/int_sqrt.h"   /* int_sqrt(idx_t n) → floor(sqrt(n)) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1 – safety */

    idx_t cumulative = 0;   /* number of elements before the current row */
    idx_t r = 1;            /* current row number   */
    idx_t kmax = 0;         /* number of columns in row r */

    /* Locate the row containing the nth entry */
    while (1) {
        idx_t s = int_sqrt(8 * r + 1);          /* floor(sqrt(8r+1))  */
        kmax = (s - 1) / 2;                     /* floor((sqrt(8r+1)-1)/2) */
        if (cumulative + kmax >= n) break;      /* found the row  */
        cumulative += kmax;                      /* add whole row to total   */
        ++r;                                     /* advance to next row       */
    }

    /* Column index within the found row */
    idx_t k = n - cumulative;    /* 1‑based column within row r */

    /* Evaluate T(r,k) = ceil( (r+1)/k - (k+1)/2 ) */
    seq_t rr = (seq_t)(r + 1);          /* r+1               */
    seq_t kk = (seq_t)k;                /* k                 */
    seq_t a_num = 2 * rr - kk * (kk + 1); /* numerator of rational number */
    seq_t b_den = 2 * kk;                /* denominator */
    seq_t val = (a_num + b_den - 1) / b_den; /* ceil division */

    return val;
}
