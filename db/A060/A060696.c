/*
 * A060696  Number of permutations in S_n avoiding the strings 123, 321 and
 *          231.
 * https://oeis.org/A060696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)!! + (n-2)!! for n ≥ 2 with a(0) = 1, a(1) = 1 (the
 *            double factorials for n = –1 and n = –2 are conventionally
 *            defined as 1)
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 20, u64 n <= 34, u128 n <= 57,
 *            big any n
 * Helpers:   double_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/double_factorial.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)2;
    /* For n ≥ 3 use the closed form */
    seq_t t1 = double_factorial(n - (idx_t)1);
    seq_t t2 = double_factorial(n - (idx_t)2);
    return t1 + t2;
}
