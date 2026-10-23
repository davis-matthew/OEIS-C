/*
 * A173245  O.g.f. (x + 1)^10/ (sum_{i=0..10} x^i) .
 * https://oeis.org/A173245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   not stated
 * Range:     i8 n <= 50, i16 n <= 50, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A173245
   Periodic with period 11 after the first term.
   a(0) = 1
   For n > 0 :
       a(n) = p[(n-1) mod 11]
   where the periodic array p (indices 0..10) is
       { 9, 35, 75, 90, 42, -42, -90, -75, -35, -9, 0 }
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    static const seq_t periodic[11] = {
        (seq_t)9,   (seq_t)35,  (seq_t)75,  (seq_t)90,  (seq_t)42,
        (seq_t)-42, (seq_t)-90, (seq_t)-75, (seq_t)-35, (seq_t)-9,
        (seq_t)0
    };

    return periodic[(size_t)((n - 1) % (idx_t)11)];
}
