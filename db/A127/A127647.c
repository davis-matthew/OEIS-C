/*
 * A127647  Triangle read by rows: row n consists of n-1 zeros followed by
 *          Fibonacci(n).
 * https://oeis.org/A127647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = fib(r) if n = r(r+1)/2 (the end of row r), otherwise 0.
 *            Here fib is the standard Fibonacci function with F(0)=0, F(1)=1
 * Range:     u8 n <= 104, u16 n <= 324, u32 n <= 1175, u64 n <= 4464,
 *            u128 n <= 5050, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>
#include "../utils/fib.h"   /* gives seq_t fib(idx_t n) */

seq_t a(idx_t n)
{
    /* Order of operations: use idx_t for arithmetic, but
       cast to seq_t when computing triangular numbers.          */
    if (n < 1)
        return (seq_t)0;           /* below offset */

    /* Find row r such that r(r+1)/2 >= n, via binary search on idx_t. */
    idx_t low = 1, high = 1;
    seq_t n_seq = (seq_t)n;

    /* Grow 'high' until the triangular number reaches or exceeds n. */
    while (1) {
        seq_t tri = (seq_t)high * (high + 1) / 2;
        if (tri >= n_seq)
            break;
        if (high > (UINT64_MAX >> 1))          /* avoid overflow */
            high = UINT64_MAX;
        else
            high <<= 1;
    }

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        seq_t tri_mid = (seq_t)mid * (mid + 1) / 2;
        if (tri_mid >= n_seq)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                        /* candidate row */

    /* Check if n is exactly the last element of row r. */
    seq_t tri = (seq_t)r * (r + 1) / 2;
    if (tri == n_seq)
        return fib(r);                    /* Fibonacci(r) */
    else
        return (seq_t)0;
}
