/*
 * A064847  Sequence a(n) such that there is a sequence b(n) with a(1) = b(1)
 *          = 1, a(n+1) = a(n) * b(n) and b(n+1) = a(n) + b(n) for n >= 1.
 * https://oeis.org/A064847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 8, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements A064847:
   a(1) = 1, a(2) = 1,
   a(n+1) = a(n) * b(n)      for n ≥ 1
   b(1) = 1,
   b(n+1) = a(n) + b(n)      for n ≥ 1
   The returned value a(n) is the denominator of the reduced fraction
   obtained by repeatedly applying f(a/b) = (a+b)/(a*b) starting from 1/1. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    seq_t a_cur = (seq_t)1;
    seq_t b_cur = (seq_t)1;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t a_next = a_cur * b_cur;
        seq_t b_next = a_cur + b_cur;
        a_cur = a_next;
        b_cur = b_next;
    }
    return a_cur;
}
