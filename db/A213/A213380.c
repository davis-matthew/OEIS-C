/*
 * A213380  a(n) = 132*binomial(n,12).
 * https://oeis.org/A213380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 15, u32 n <= 28, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 132 * binomial(n,12)
 * Sequence offset is 0 (a(0) = 0).
 */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* The binomial routine returns 0 for n < 12, so no special case is
       required.  Use a cast to guarantee the literal 132 is treated as
       a seq_t value. */
    return (seq_t)132 * binomial(n, 12);
}
