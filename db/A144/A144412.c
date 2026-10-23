/*
 * A144412  Invert transform of odd nonprime gaps adjusted to be from the set
 *          {2,1,0,-1}: b(n)=A067970(n)/2-2; a(n)=Sum[b(n + 1)*a(n - k), {k,
 *          1, n}].
 * https://oeis.org/A144412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  uned, sign
 * Formula:   a(n) = 2 for n=1,2; a(3)=4; a(4)=0; a(5)=−8; a(n)=0 for n≥6
 * Range:     i8 n <= 101, i16 n <= 101, i32 n <= 101, i64 n <= 101,
 *            i128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)4;
    if (n == 4) return (seq_t)0;
    if (n == 5) return (seq_t)-8;
    return (seq_t)0;
}
