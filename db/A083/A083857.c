/*
 * A083857  Square array T(n,k) of binomial transforms of generalized
 *          Fibonacci numbers, read by ascending antidiagonals, with n, k >=
 *          0.
 * https://oeis.org/A083857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A083857: read the square array T(n,k) (with rows n ≥ 0 and columns k ≥ 0) by ascending antidiagonals.
 * The entries are defined by
 *    T(n,0) = 0,                         T(n,1) = 1
 *    T(n,k) = 3*T(n,k-1) + (n-2)*T(n,k-2)   for k ≥ 2
 * The function a(n) returns the n‑th term (0‑based) in the antidiagonal ordering.
 *
 * The implementation computes the coordinates (i,j) of the n‑th cell and then evaluates
 * T(i,j) by the above recurrence.  No floating point or external dependencies are used.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /*  Find the row/diagonal 's' and column offset 'j' for linear index n  */
    idx_t s = 0;          /* current diagonal sum i+j = s   */
    idx_t cum = 0;        /* number of cells before diagonal s   */

    /*  Count diagonals until the cumulative count exceeds n   */
    while (cum + s + 1 <= n) {
        cum += s + 1;     /* cells in diagonal s      */
        s++;
    }

    /*  Now cum + s + 1 > n  */
    idx_t j = n - cum;        /* column   (0 ≤ j ≤ s)   */
    idx_t i = s - j;          /* row      (0 ≤ i ≤ s)   */

    /*  Evaluate T(i,j) by the recurrence  */
    if (j == 0) return 0;          /* T(i,0) = 0 */
    if (j == 1) return 1;          /* T(i,1) = 1 */

    seq_t b0 = 0;    /* T(i,0) */
    seq_t b1 = 1;    /* T(i,1) */
    seq_t bk;

    /* recurrence: T(i,k) = 3*T(i,k-1) + (i-2)*T(i,k-2)  */
    /*  To avoid signed arithmetic, rewrite as 3*b1 + i*b0 – 2*b0  */
    for (idx_t k = 2; k <= j; k++) {
        bk = 3 * b1 + i * b0 - 2 * b0;
        b0 = b1;
        b1 = bk;
    }
    return b1;
}
