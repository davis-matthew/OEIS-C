/*
 * A078336  Duplicate of A076441.
 * https://oeis.org/A078336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 1+u(n), u(1)=u(2)=u(3)=1, u(n) =
 *            sign(u(n-1)-u(n-2))*u(n-3); duplicate of A076441, periodic with
 *            period 9 from n=4
 * Range:     i8 n <= 35, i16 n <= 35, i32 n <= 35, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 35 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A076441_a(idx_t n)
{
    static const int A076441_pat[9] = {1, 0, 0, 1, 0, 2, 1, 2, 2};
    if (n <= 3U)
        return (seq_t)2;
    return (seq_t)A076441_pat[(n - 4U) % 9U];
}

seq_t a(idx_t n)
{
    return A076441_a(n);
}
