/*
 * A111075  a(n) = F(n) * Sum_{k|n} 1/F(k), where F(k) is the k-th Fibonacci
 *          number.
 * https://oeis.org/A111075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = F(n) * Sum_{k|n} 1/F(k) where F(k) is the k‑th Fibonacci
 *            number. The implementation uses fast doubling to compute each
 *            Fibonacci value on demand. It is O(d(n)·log n) per call, where
 *            d(n) is the number of divisors of n
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 47, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Helpers:   fib_fast
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib_fast.h"   /* provides void fib_fast(idx_t n, seq_t *f, seq_t *f_next); */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for non‑positive indices */

    /* compute F(n) using fast doubling */
    seq_t Fn, Fn_next;
    fib_fast(n, &Fn, &Fn_next);    /* Fn holds F(n) */

    seq_t sum = 0;

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* divisor d */
            seq_t fd, fd_next;
            fib_fast(d, &fd, &fd_next);
            sum += Fn / fd;

            idx_t e = n / d;
            if (e != d) {          /* matching divisor e */
                seq_t fe, fe_next;
                fib_fast(e, &fe, &fe_next);
                sum += Fn / fe;
            }
        }
    }

    return sum;
}
