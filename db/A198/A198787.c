/*
 * A198787  Triangle read by rows: T(n,k) is the number of edges in the
 *          (n,k)-Turán graph.
 * https://oeis.org/A198787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, easy, nonn
 * Formula:   T(n,k) = (n² - (n mod k)·ceil(n/k)² - (k-(n mod
 *            k))·floor(n/k)²)/2 The sequence a(n) lists T(n,k) by rows:
 *            a(1)=T(1,1), a(2)=T(2,1), a(3)=T(2,2), a(4)=T(3,1), … Hence, for
 *            each n we interpret it as the (row = r, column = c) position in
 *            the triangle where r(r+1)/2 ≥ n > (r-1)r/2
 * Range:     u8 n <= 284, u16 n <= 9865, u32 n <= 9865, u64 n <= 9865,
 *            u128 n <= 9865, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence */
    /* Find the row r such that r(r+1)/2 >= n. */
    idx_t r = 1;
    seq_t sum = (seq_t)1;                 /* triangular number r(r+1)/2 */
    while (sum < (seq_t)n) {
        r++;
        sum += (seq_t)r;                  /* sum becomes r(r+1)/2 */
    }

    /* Previous triangular sum before this row: (r-1)r/2 */
    seq_t prevSum = sum - (seq_t)r;

    /* Column within the row (1‑based) */
    idx_t c = (idx_t)((seq_t)n - prevSum);

    /* Work with the Turán graph parameters: n = r, k = c */
    seq_t r_seq = (seq_t)r;
    seq_t c_seq = (seq_t)c;

    seq_t rem        = r_seq % c_seq;                    /* r mod k */
    seq_t floor_d    = r_seq / c_seq;                    /* floor(r/k) */
    seq_t ceil_d     = (r_seq + c_seq - (seq_t)1) / c_seq;/* ceil(r/k)  */

    /* Compute T(r,c) using the formula. */
    seq_t term = (r_seq * r_seq
                  - rem * ceil_d * ceil_d
                  - (c_seq - rem) * floor_d * floor_d)
                 / (seq_t)2;

    return term;
}
