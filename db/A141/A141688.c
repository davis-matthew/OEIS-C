/*
 * A141688  Triangle T(n, k) = Fibonacci(2*k)*T(n-1, k) +
 *          Fibonacci(2*(n-k+1))*T(n-1, k-1), with T(n, 1) = T(n, n) = 1, read
 *          by rows.
 * https://oeis.org/A141688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the triangle read row‑wise defined by T(n,k) =
 *            Fibonacci(2*k)*T(n-1,k) + Fibonacci(2*(n-k+1))*T(n-1,k-1),
 *            T(n,1)=T(n,n)=1. Offset is 1
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 48, u64 n <= 96, u128 n <= 179,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Find the row r containing the n‑th element: r*(r+1)/2 >= n */
    idx_t r = 1;
    idx_t cumulative = 1;                 /* sum 1..r */
    while (cumulative < n) {
        ++r;
        cumulative += r;
    }

    /* Allocate two rows (indices 1..r) */
    idx_t rowSize = r + 2;                /* small slack */
    seq_t *prev = (seq_t *)malloc(rowSize * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc(rowSize * sizeof(seq_t));
    if (!prev || !curr) {
        /* allocation failure – return 0 as a safe sentinel */
        free(prev);
        free(curr);
        return (seq_t)0;
    }

    /* Base row 1 */
    prev[1] = 1;

    /* Build rows 2 .. r */
    for (idx_t row = 2; row <= r; ++row) {
        curr[1] = 1;
        curr[row] = 1;
        for (idx_t k = 2; k < row; ++k) {
            seq_t f1 = fib(2 * k);
            seq_t f2 = fib(2 * (row - k + 1));
            curr[k] = f1 * prev[k] + f2 * prev[k - 1];
        }
        /* Rotate buffers for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* Index of element in the found row (0‑based) */
    idx_t prevTotal = (r - 1) * r / 2;     /* elements before row r */
    idx_t offset   = n - prevTotal - 1;    /* 0‑based in this row */

    seq_t result = prev[offset + 1];

    free(prev);
    free(curr);
    return result;
}
