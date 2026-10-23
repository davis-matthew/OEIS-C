/*
 * A125736  a(n)=IntegerLog(n-1)+IntegerLog(n+1), where
 *          IntegerLog(n)=A001414(n).
 * https://oeis.org/A125736
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = IntegerLog(n-1) + IntegerLog(n+1) IntegerLog(m) = sum
 *            over prime factors p^e of (p*e) (sequence A001414)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   integer_log
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/integer_log.h"

seq_t a(idx_t n)
{
    /* n >= 3 as per sequence offset */
    return integer_log(n - 1) + integer_log(n + 1);
}
