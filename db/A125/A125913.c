/*
 * A125913  Sprague-Grundy values for octal game .144.
 * https://oeis.org/A125913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=0, a(3)=0, a(4)=1. For n ≥ 5, a(n) is periodic with
 *            period 10: a(5..14) = 2,2,2,2,4,4,1,1,1,1 and repeats thereafter
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2 || n == 3) return (seq_t)0;
    if (n == 4) return (seq_t)1;
    static const seq_t period[10] =
        { (seq_t)2, (seq_t)2, (seq_t)2, (seq_t)2,
          (seq_t)4, (seq_t)4, (seq_t)1, (seq_t)1,
          (seq_t)1, (seq_t)1 };
    return period[(size_t)(n - 5) % 10];
}
