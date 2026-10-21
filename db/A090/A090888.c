/*
 * A090888  Matrix defined by a(n,k) = 3^n*Fibonacci(k) - 2^n*Fibonacci(k-2),
 *          read by antidiagonals.
 * https://oeis.org/A090888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 78, u32 n <= 253, u64 n <= 903,
 *            u128 n <= 3403, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A090888 : read antidiagonals of the matrix
 *  a(n,0) = 2^n
 *  a(n,1) = 3^n – 2^n
 *  a(n,k) = a(n,k‑1) + a(n,k‑2)   (k ≥ 2)
 *  The sequence a(n) is obtained by traversing that matrix
 *  along antidiagonals, starting at (0,0):
 *      a(0) = a(0,0), a(1) = a(1,0), a(2) = a(0,1), a(3) = a(2,0), …
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* find the diagonal d that contains index n
     * cumulative entries up to diagonal d-1:  d(d+1)/2
     * use a simple incremental search (O(√n)) */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n) ++d;

    idx_t diag_start = d * (d + 1) / 2;
    idx_t offset     = n - diag_start;          /* 0 … d   */
    idx_t row = d - offset;  /* row index of the entry */
    idx_t col = offset;       /* column index of the entry */

    /* compute 2^row and 3^row by repeated multiplication */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < row; ++i) pow2 *= (seq_t)2;

    seq_t pow3 = (seq_t)1;
    for (idx_t i = 0; i < row; ++i) pow3 *= (seq_t)3;

    /* compute the required Fibonacci numbers */
    seq_t fib_k       = 0;     /* fib(col)   */
    seq_t fib_k_minus = 0;     /* fib(col‑2) */

    if (col == 0) {
        fib_k = (seq_t)0;
        /* fib_k_minus unused */
    } else if (col == 1) {
        fib_k = (seq_t)1;
    } else if (col == 2) {          /* special small cases for speed */
        fib_k       = (seq_t)1;   /* fib(2) */
        fib_k_minus = (seq_t)0;   /* fib(0) */
    } else if (col == 3) {
        fib_k       = (seq_t)2;   /* fib(3) */
        fib_k_minus = (seq_t)1;   /* fib(1) */
    } else {                       /* general case: col ≥ 4 */
        seq_t a0 = (seq_t)0;      /* fib(0)   */
        seq_t a1 = (seq_t)1;      /* fib(1)   */
        for (idx_t i = 2; i <= col; ++i) {
            seq_t a2 = a0 + a1;   /* fib(i)   */
            a0 = a1;
            a1 = a2;
            if (i == col - 2) fib_k_minus = a1;  /* fib(col‑2) */
            if (i == col)     fib_k       = a1;  /* fib(col)   */
        }
    }

    /* compute the matrix entry a(row,col) */
    seq_t value;
    if (col == 0) {
        value = pow2;
    } else if (col == 1) {
        value = pow3 - pow2;
    } else {
        value = pow3 * fib_k - pow2 * fib_k_minus;
    }

    return value;
}
