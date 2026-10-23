/*
 * A128213  Expansion of (1-x+2x^2-2x^3)/(1-x+x^2)^2.
 * https://oeis.org/A128213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 63, i16 n <= 63, i32 n <= 63, i64 n <= 63, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  a(n) =
 *   {  (-1)^(n/3)                       , if n % 3 == 0
 *   {  (-1)^(⌊n/3⌋) * (n - (n%3==2?1:0)), if n % 3 != 0
 * This follows from the recurrence y(m+1,n)=y(m,n)-y(m-1,n) with
 * y(0,n)=1, y(1,n)=n, and a(n)=y(n,n).
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t mod3 = n % 3;
    if (mod3 == 0) {
        uint64_t k = n / 3;
        return (k & 1) ? (seq_t)-1 : (seq_t)1;
    } else {
        uint64_t k = n / 3;
        seq_t base = (mod3 == 1) ? (seq_t)n : (seq_t)(n - 1);
        return (k & 1) ? -base : base;
    }
}
