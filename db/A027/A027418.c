/*
 * A027418  Duplicate of A035508.
 * https://oeis.org/A027418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = F_{2n} - 1, where F_k is the Fibonacci sequence (F_0 = 0,
 *            F_1 = 1)
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 14, i64 n <= 14, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);
    seq_t c = a * (((b << 1) - a));
    seq_t d = a * a + b * b;
    if (n & 1) {
        *fn  = d;
        *fn1 = c + d;
    } else {
        *fn  = c;
        *fn1 = d;
    }
}

seq_t a(idx_t n)
{
    idx_t m = n << 1;          /* m = 2n */
    seq_t fn, fn1;
    fib_pair(m, &fn, &fn1);    /* fn = F_{2n} */
    return fn - (seq_t)1;
}
