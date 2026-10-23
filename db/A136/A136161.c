/*
 * A136161  a(n) = 2*a(n-3) - a(n-6), starting a(0..5) = 0, 5, 2, 1, 3, 1.
 * https://oeis.org/A136161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-3) - a(n-6) Initial values: a(0)=0, a(1)=5, a(2)=2,
 *            a(3)=1, a(4)=3, a(5)=1
 * Range:     i8 n <= 201, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t base[6] = { (seq_t)0, (seq_t)5, (seq_t)2,
                               (seq_t)1, (seq_t)3, (seq_t)1 };

seq_t a(idx_t n)
{
    if (n < 6) return base[n];

    /* A sliding window of the last six values is enough to compute the
       recurrence for any n. The window is kept in arr[0..5] where
       arr[i] holds a(i) for some i modulo 6. */
    seq_t arr[6];
    for (idx_t i = 0; i < 6; ++i) arr[i] = base[i];

    for (idx_t i = 6; i <= n; ++i) {
        /* arr[(i-3)%6] is a(i-3), arr[(i-6)%6] is a(i-6) */
        seq_t val = 2 * arr[(i - 3) % 6] - arr[(i - 6) % 6];
        arr[i % 6] = val;
    }

    return arr[n % 6];
}
