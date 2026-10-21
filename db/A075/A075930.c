/*
 * A075930  Positions of check bits in code in A075928.
 * https://oeis.org/A075930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the (n+1)-th positive integer that has an odd Hamming
 *            weight (number of 1‑bits in its binary representation) and is
 *            not a power of two. The sequence is indexed starting at n = 0
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef _A075930_H
#define _A075930_H

/* Compute parity of a 64‑bit integer: returns 1 if number of 1‑bits is odd. */
static inline int parity64(idx_t x)
{
    int p = 0;
    while (x) {
        p ^= (int)(x & 1);
        x >>= 1;
    }
    return p;
}

seq_t a(idx_t n)
{
    idx_t count = 0;        /* numbers found so far with desired property   */
    idx_t val   = 1;        /* candidate integer to test                    */

    while (1) {
        /* Skip powers of two: x & (x-1) == 0 iff x is a power of two or zero */
        if ((val & (val - 1)) != 0) {
            if (parity64(val)) {            /* odd number of 1‑bits         */
                if (count == n)
                    return (seq_t)val;     /* desired element found        */
                ++count;                   /* increment count of valid items*/
            }
        }
        ++val;
    }
}

#endif /* _A075930_H */
