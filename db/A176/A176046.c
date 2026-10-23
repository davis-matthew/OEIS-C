/*
 * A176046  Triangle T(n,m) = 1 + F(m) + F(n-m) - F(n), read by rows, where F
 *          = A000045 is the Fibonacci sequence.
 * https://oeis.org/A176046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   T(r,m) = 1 + F(m) + F(r-m) - F(r) a(n) is the flattening by
 *            rows: n = r(r+1)/2 + m , 0 ≤ m ≤ r. Fibonacci F(0)=0, F(1)=1
 * Range:     i8 n <= 65, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* iterative Fibonacci, works for arbitrary‐precision seq_t */
static seq_t fib_u(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    seq_t a = (seq_t)0;
    seq_t b = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t t = a + b;
        a = b;
        b = t;
    }
    return b;
}

seq_t a(idx_t n)
{
    /* Find the largest r such that r(r+1)/2 ≤ n */
    idx_t low = 0;
    idx_t high = 1;
    while (1) {
        seq_t val = (seq_t)high * (seq_t)(high + 1) / 2;
        if (val > n) break;
        high <<= 1;
    }
    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        seq_t val = (seq_t)mid * (seq_t)(mid + 1) / 2;
        if (val <= n)
            low = mid;
        else
            high = mid;
    }
    idx_t r = low;
    seq_t offset = (seq_t)r * (seq_t)(r + 1) / 2;
    idx_t m = n - (idx_t)offset;

    seq_t fm = fib_u(m);
    seq_t frm = fib_u(r - m);
    seq_t fr = fib_u(r);

    return (seq_t)1 + fm + frm - fr;
}
