/*
 * A230107  Define a sequence by b(1)=n, b(k+1)=b(k)+(sum of digits of b(k));
 *          a(n) is the number of steps needed to reach a term in A004207, or
 *          a(n) = -1 if the sequence never joins A004207.
 * https://oeis.org/A230107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   with m = n+1, b(1)=m, b(k+1)=b(k)+digitsum(b(k)); a(n) = number
 *            of steps until b hits a term of A004207 (merge of two increasing
 *            sequences), or -1 if 3 divides m
 * Range:     i8 n <= 3, i16 n <= 69, i32 n <= 69, i64 n <= 69, i128 n <= 69,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 69 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A230107_dsum(seq_t v) {
    seq_t s = 0;
    while (v > 0) {
        s += v % 10;
        v /= 10;
    }
    return s;
}

seq_t a(idx_t n) {
    seq_t m = (seq_t)n + 1;
    if (m % 3 == 0) return (seq_t)(-1);

    seq_t b = m;
    seq_t t = 1; /* A004207 terms: 1, 2, 4, 8, 16, 23, ... */
    long long steps = 0;
    long long guard = 0;
    while (guard < 100000000LL) {
        while (t < b) {
            t += A230107_dsum(t);
        }
        if (t == b) return (seq_t)steps;
        b += A230107_dsum(b);
        steps++;
        guard++;
    }
    return (seq_t)(-1);
}
