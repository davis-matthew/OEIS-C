/*
 * A164898  First differences of numbers having only odd digits in their
 *          decimal representation.
 * https://oeis.org/A164898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A014261(n+1) - A014261(n)
 * Range:     u8 n <= 279, u16 n <= 7029, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   odd_num
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/odd_num.h"

seq_t a(idx_t n)
{
    seq_t prev = odd_num(n);
    seq_t next = odd_num(n + 1);
    return next - prev;
}
