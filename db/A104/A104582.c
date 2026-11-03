/*
 * A104582  Triangle read by rows: T(i,j) is the (i,j)-entry (1 <= j <= i) of
 *          the product of the lower triangular matrix (Fibonacci(i-j+1)) and
 *          of the lower triangular matrix all of whose entries are equal to 1
 *          (for j <= i).
 * https://oeis.org/A104582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Fibonacci(row - col + 3) - 1 where (row,col) is the
 *            position of n in the triangle read by rows (row starts at 1, col
 *            starts at 1)
 * Range:     u8 n <= 66, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Validated: partial, pass, 81 terms through n = 81 (2026-11-01)
 * Helpers:   fibonacci
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fibonacci.h"

/* Fast‑doubling Fibonacci:
   fib_doubling(k, &F(k), &F(k+1)) returns Fibonacci numbers. */
static inline void fib_doubling(idx_t k, seq_t *f, seq_t *f_next)
{
    if (k == 0) {
        *f = (seq_t)0;
        *f_next = (seq_t)1;
        return;
    }
    seq_t a, b;
    fib_doubling(k >> 1, &a, &b);
    /* a = F(k), b = F(k+1) */
    seq_t two_b = b + b;        /* 2 * F(k+1) */
    seq_t c = a * (two_b - a);  /* F(2k)   = F(k)*(2*F(k+1)-F(k)) */
    seq_t d = a * a + b * b;    /* F(2k+1) = F(k)^2 + F(k+1)^2 */
    if ((k & 1) == 0) {
        *f = c;
        *f_next = d;
    } else {
        *f = d;
        *f_next = c + d;
    }
}

/* Compute the minimal r ≥ 1 with r(r+1)/2 ≥ n. */
static inline idx_t row_of_triangle(idx_t n)
{
    if (n <= 1) return 1;
    idx_t low = 1, high = 1;
    /* Grow high until it surely contains n. */
    while (((unsigned __int128)high * (high + 1)) / 2 < n)
        high <<= 1;
    /* Binary search for the smallest r. */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 tri = ((unsigned __int128)mid * (mid + 1)) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* OEIS sequence A104582. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* out‑of‑range is harmless */

    idx_t row = row_of_triangle(n);
    unsigned __int128 before = ((unsigned __int128)row * (row - 1)) / 2;
    idx_t col = n - (idx_t)before;        /* 1‑based column in the row */
    idx_t m   = row - col + 3;            /* index for Fibonacci */

    seq_t f = fibonacci(m);
    return f - (seq_t)1;
}
