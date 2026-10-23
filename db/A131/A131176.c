/*
 * A131176  a(n) = (n^5-n-10)/10.
 * https://oeis.org/A131176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n^5 - n - 10) / 10
 * Range:     i8 n <= 4, i16 n <= 12, i32 n <= 37, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the n-th term of OEIS sequence A131176.
   n is an unsigned 64‑bit index. Result is returned as seq_t,
   which may be any suitably sized integer type or arbitrary‑precision
   integer class. */
seq_t a(idx_t n)
{
    /* Use a wider integer type for the intermediate calculation to avoid
       overflow for common integer sizes (int32, int64).  The builtin type
       __int128 is available in C11/C++17 for 128‑bit signed arithmetic. */
    __int128 x = n;
    __int128 val = x * x * x * x * x - x - 10;
    val /= 10;
    return (seq_t)val;
}
