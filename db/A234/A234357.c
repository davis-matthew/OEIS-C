/*
 * A234357  Array A(n,k) by ascending antidiagonals: A(n,k) = n^k *
 *          Fibonacci(k + 1) for n, k >= 1.
 * https://oeis.org/A234357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 50, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   big_pow, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A234357:
   a(n) = n^k * Fibonacci(k+1) where (n,k) are the integers on the
   n‑th antidiagonal of the infinite array A(n,k).  The function
   maps the global index n to the corresponding (row,col) and
   evaluates A(row,col). */

#include "../utils/big_pow.h"
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* unused, offset starts at 1 */

    /* Find the diagonal s such that s*(s+1)/2 >= n   (O(log n)) */
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid * (mid + 1) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t s = lo;               /* diagonal index (1‑based) */

    idx_t prev = s * (s - 1) / 2;      /* elements before this diagonal   */
    idx_t offset = n - prev - 1;       /* 0‑based index inside diagonal   */

    idx_t row = s - offset;        /* row coordinate (>=1)             */
    idx_t col = offset + 1;        /* column coordinate (>=1)          */

    seq_t pow_val = big_pow((seq_t)row, (seq_t)col);          /* row^col       */
    seq_t fib_val = fib((idx_t)(col + 1));                    /* Fibonacci(col+1) */
    return pow_val * fib_val;
}
