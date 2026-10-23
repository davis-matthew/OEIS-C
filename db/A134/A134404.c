/*
 * A134404  Triangle read by rows in which row n contains Fib(0), ...,
 *          Fib(n-1), Fib(n), Fib(n-1), ..., Fib(0).
 * https://oeis.org/A134404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = Fib(k) where r = floor(sqrt(n)); offset = n - r²; k =
 *            (offset <= r) ? offset : 2*r - offset; Fib(0)=0, Fib(1)=1
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root floor(sqrt(x)) for unsigned 64‑bit values. */
static idx_t int_sqrt(idx_t x)
{
    idx_t lo = 0, hi = x, mid, ans = 0;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        /* avoid division by zero */
        if (mid != 0 && mid > x / mid) {
            hi = mid - 1;
        } else {
            ans = mid;
            lo = mid + 1;
        }
    }
    return ans;
}

/* Compute Fibonacci number Fib(k) with Fib(0)=0, Fib(1)=1.
   The value fits in seq_t (which may be arbitrary precision). */
static seq_t fib_seq(idx_t k)
{
    if (k == 0) return (seq_t)0;
    if (k == 1) return (seq_t)1;
    seq_t a = 0, b = 1, c;
    for (idx_t i = 2; i <= k; ++i) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

seq_t a(idx_t n)
{
    idx_t r = int_sqrt(n);
    idx_t offset = n - r * r;
    idx_t k = (offset <= r) ? offset : 2 * r - offset;
    return fib_seq(k);
}
