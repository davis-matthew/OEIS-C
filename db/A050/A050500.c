/*
 * A050500  Integer part of n/log(n).
 * https://oeis.org/A050500
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( n / log(n) ) (offset 2)
 * Range:     u8 n <= 1937, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n < 2)                  /* sequence starts at n = 2 */
        return 0;              /* undefined below the offset */

    /* n is uint64_t; convert to double for the logarithm */
    double nd = (double)n;
    double val = nd / log(nd); /* natural logarithm */

    /* integer part as a uint64_t (the result is < 2^32 for n ≤ 10000) */
    uint64_t res = (uint64_t)val;  /* truncates toward zero */

    /* cast to the build‑specific seq_t (works for built‑in integer
       types and the arbitrary‑precision big integer class) */
    return (seq_t)res;
}
