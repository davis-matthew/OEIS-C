/*
 * A240461  Number of 2 X n 0..3 arrays with no element equal to one plus the
 *          sum of elements to its left or three plus the sum of the elements
 *          above it or one plus the sum of the elements diagonally to its
 *          northwest, modulo 4.
 * https://oeis.org/A240461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 6*a(n-1) - 7*a(n-2) - 4*a(n-3) + 8*a(n-4)
 * Initial values: a(1)=5, a(2)=21, a(3)=89, a(4)=375
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* If n is less than 1, return 0 (out of offset). */
    if (n == 0)
        return 0;

    /* Pre‑computed initial terms */
    static const seq_t init[4] = {5, 21, 89, 375};

    if (n <= 4)
        return init[n - 1];

    /* Sliding window for the last four values */
    seq_t a_nm4 = init[0]; /* a(n-4) */
    seq_t a_nm3 = init[1]; /* a(n-3) */
    seq_t a_nm2 = init[2]; /* a(n-2) */
    seq_t a_nm1 = init[3]; /* a(n-1) */
    seq_t a_n;             /* a(n) */

    for (idx_t i = 5; i <= n; ++i)
    {
        /* Compute 6*a(n-1) - 7*a(n-2) - 4*a(n-3) + 8*a(n-4) */
        seq_t sum_pos = 6 * a_nm1 + 8 * a_nm4;
        seq_t sum_neg = 7 * a_nm2 + 4 * a_nm3;
        a_n = sum_pos - sum_neg;   /* sum_pos ≥ sum_neg for all n ≥ 5 */

        /* Shift the window for the next iteration */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
