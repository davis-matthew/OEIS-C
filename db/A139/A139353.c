/*
 * A139353  Let the binary expansion of n be n = Sum_{k} 2^{r_k}, let e(n) be
 *          the number of r_k's that are even, o(n) the number that are odd;
 *          sequence gives e(n)*o(n).
 * https://oeis.org/A139353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (# of set bits at even exponents) * (# of set bits at odd
 *            exponents)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where exponents are 0-based (2^0 is the least‑significant bit). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Count bits in even and odd positions */
    unsigned int e = 0;
    unsigned int o = 0;
    unsigned int pos = 0;          /* Current bit position (0 = least significant) */
    idx_t x = n;

    while (x != 0) {
        if (x & 1) {               /* bit is set at current position */
            if (pos & 1)           /* odd exponent */
                ++o;
            else
                ++e;                /* even exponent */
        }
        x >>= 1;
        ++pos;
    }

    return (seq_t)e * (seq_t)o;
}
