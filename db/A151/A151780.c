/*
 * A151780  a(n) = 5^(wt(n) - 1) where wt(n) = A000120(n).
 * https://oeis.org/A151780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5^(popcount(n) - 1) (OEIS A151780)
 * Range:     u8 n <= 30, u16 n <= 254, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline unsigned popcount_idx(idx_t v)
{
    unsigned c = 0;
    while (v)
    {
        v &= v - 1;
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n == 0 we return 1
       to keep the result well‑defined, though it is outside the
       supported index range. */
    if (n == 0)
        return (seq_t)1;

    unsigned pc   = popcount_idx(n);   /* population count of n */
    unsigned exp  = pc - 1;            /* exponent for 5 */

    seq_t res = (seq_t)1;              /* 5^0 == 1 */
    seq_t base = (seq_t)5;
    while (exp--)
        res *= base;

    return res;
}
