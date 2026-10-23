/*
 * A008502  8-dimensional centered tetrahedral numbers.
 * https://oeis.org/A008502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 44, u64 n <= 731, u128 n <= 1000,
 *            big any n
 * Helpers:   binom9_basic
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom9_basic.h"
/*  a(n) = binom(n+9,9) – binom(n,9)
 *
 *  The binomial coefficient is computed by a simple iterative
 *  Pascal‑table method that uses only integer additions.
 *  For any n where the result fits in seq_t (including 64‑bit) the
 *  intermediate values never overflow, because binom(k,9) is monotone
 *  increasing and never exceeds the final value.
 *
 *  a(0) = 1  (since binom(9,9)=1, binom(0,9)=0)
 *  a(1) = 10, a(2) = 55, …
 */

seq_t a(idx_t n)
{
    seq_t high = binom9_basic(n + 9u);
    seq_t low  = binom9_basic(n);
    return high - low;
}
