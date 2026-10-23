/*
 * A143929  Eigentriangle by rows, termwise products of the natural numbers
 *          decrescendo and the bisected Fibonacci series.
 * https://oeis.org/A143929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = Let r be the smallest integer with r(r+1)/2 >= n Let c =
 *            n - r(r-1)/2 If c == 1 -> value = r Else -> value = F(2*(c-1)) *
 *            (r - c + 1) where F is the Fibonacci sequence A000045. (The
 *            triangle is T(n,m): T(n,1)=n, T(n,m)=F(2*(m-1))*(n-m+1) for
 *            m>=2)
 * Range:     u8 n <= 34, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"   /* provides seq_t fib(idx_t) */

/* Find smallest r such that r*(r+1)/2 >= n */
static idx_t find_row(idx_t n)
{
    idx_t low = 0;
    idx_t high = 1;

    /* Exponential search to get an upper bound */
    while (1) {
        unsigned __int128 tri = (unsigned __int128)high * (high + 1) / 2;
        if (tri >= n) break;
        high <<= 1;
    }

    /* Binary search between low and high */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri_mid = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri_mid >= n)
            high = mid;
        else
            low = mid;
    }
    return high;   /* r */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* Undefined for n<1; return 0 for safety */

    /* Determine row r and column c */
    idx_t r = find_row(n);

    unsigned __int128 prev_tri = (unsigned __int128)(r - 1) * r / 2;
    idx_t c = n - (idx_t)prev_tri;   /* column within the row, 1-indexed */

    if (c == 1) {
        /* First element of the row */
        return (seq_t)r;
    }

    /* Compute Fibonacci value: F(2*(c-1)) */
    idx_t fib_index = 2 * (c - 1);
    seq_t f_val = fib(fib_index);

    /* Multiply by (r - c + 1) */
    seq_t mult = (seq_t)(r - c + 1);
    return f_val * mult;
}
