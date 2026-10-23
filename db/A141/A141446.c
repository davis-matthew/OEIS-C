/*
 * A141446  A102055(n) mod 9.
 * https://oeis.org/A141446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 85, i16 n <= 85, i32 n <= 85, i64 n <= 85, i128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A102055(n) mod 9 – periodic after the first three terms
   a(0)=1, a(1)=2, a(2)=1; for n ≥ 3 the sequence repeats every 18 values:
   4, −4, 7, −5, 8, −5, 4, −7, 1, −5, 5, −2, 4, −1, 4, −5, 2, −8
*/
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)1;

    static const seq_t period[18] = {
        (seq_t)4,  (seq_t)-4, (seq_t)7,  (seq_t)-5,
        (seq_t)8,  (seq_t)-5, (seq_t)4,  (seq_t)-7,
        (seq_t)1,  (seq_t)-5, (seq_t)5,  (seq_t)-2,
        (seq_t)4,  (seq_t)-1, (seq_t)4,  (seq_t)-5,
        (seq_t)2,  (seq_t)-8
    };

    return period[(size_t)((n - 3) % 18)];
}
