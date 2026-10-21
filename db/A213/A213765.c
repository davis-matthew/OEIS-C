/*
 * A213765  Rectangular array: (row n) = b**c, where b(h) = 2*n-1, c(h) =
 *          F(n-1+h), F=A000045 (Fibonacci numbers), n>=1, h>=1, and ** =
 *          convolution.
 * https://oeis.org/A213765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the (n)-th term of the array T(r,s) defined by T(r,s) =
 *            F(r+s+4) - 2*s*F(r+1) - F(r+4), where r and s are obtained by
 *            reading the array by falling antidiagonals. F is the Fibonacci
 *            sequence (F(0)=0, F(1)=1)
 * Range:     u8 n <= 36, u16 n <= 190, u32 n <= 903, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Helpers:   fib_iter
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib_iter.h"

seq_t a(idx_t n)
{
    /* Find s = r + s - 2 such that (s-1)*s/2 >= n */
    idx_t lo = 1, hi = 2;
    while (((unsigned __int128)(hi - 1) * hi) / 2 < n) {
        hi <<= 1;          /* double until sufficient */
    }
    while (lo < hi) {
        idx_t mid = (lo + hi) / 2;
        idx_t cnt = (unsigned __int128)(mid - 1) * mid / 2;
        if (cnt >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t sum = lo;  /* sum of r and s (row+col) */

    /* Number of terms before this antidiagonal */
    idx_t prev = (unsigned __int128)(sum - 1) * (sum - 2) / 2;
    idx_t pos   = n - prev - 1;        /* 0‑based position on the diagonal */
    idx_t r     = pos + 1;             /* row index */
    idx_t s     = sum - r;             /* column index */

    seq_t Fn_k4 = fib_iter(r + s + 4);
    seq_t Fn_1  = fib_iter(r + 1);
    seq_t Fn_4  = fib_iter(r + 4);

    seq_t term = Fn_k4 - Fn_4 - ( (seq_t)2 * (seq_t)s * Fn_1 );
    return term;
}
