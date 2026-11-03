/*
 * A152203  Triangle T(n,k) = (2n+1-2k)*fibonacci(k), read by rows.
 * https://oeis.org/A152203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = (2*r+1-2*c) * fib(c), where r = minimal integer with
 *            r(r+1)/2 >= n and c = n - (r-1)*r/2
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Using the supplied Fibonacci utility */
#include <stdint.h>
#include <stdbool.h>

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

seq_t a(idx_t n)
{
    /* Find r with r(r+1)/2 >= n (binary search) */
    idx_t low  = 1;
    idx_t high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Compute column index c */
    unsigned __int128 prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t c = n - (idx_t)prev;

    /* Offset part of the term */
    idx_t offset_idx = 2 * r + 1 - 2 * c;
    seq_t offset = (seq_t)offset_idx;

    /* Fibonacci of c */
    seq_t fib_val = fib(c);

    return offset * fib_val;
}
