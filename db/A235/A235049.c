/*
 * A235049  Subtract one from each nonzero digit in decimal representation of
 *          n.
 * https://oeis.org/A235049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base, look
 * Formula:   a(n) = subtract 1 from each non‑zero decimal digit of n
 * Range:     u8 n <= 366, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t x = n;        /* unsigned copy of the index */
    seq_t res = 0;        /* result accumulator */
    seq_t mult = 1;       /* current base‑10 multiplier */

    while (x > 0) {
        uint64_t d = x % 10;          /* current decimal digit */
        if (d != 0) d -= 1;           /* subtract 1 if non‑zero */
        res += (seq_t)d * mult;       /* add transformed digit */
        mult *= 10;                   /* shift multiplier by one decimal place */
        x /= 10;                     /* move to next digit */
    }

    /* For n == 0 the loop is skipped and res remains 0 */
    return res;
}
