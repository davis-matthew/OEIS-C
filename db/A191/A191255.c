/*
 * A191255  Fixed point of the morphism 0 -> 01, 1 -> 02, 2 -> 03, 3 -> 01.
 * https://oeis.org/A191255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9573, u16 n <= 9573, u32 n <= 9573, u64 n <= 9573,
 *            u128 n <= 9573, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A191255: Fixed point of the morphism 0->01, 1->02, 2->03, 3->01
   Formula:  a(n) = 0                     if n is odd
              a(n) = k                     if n is even,
                 where k = (v2(n) mod 3)  if non‑zero
                      or k = 3            if (v2(n) mod 3) == 0
   (v2(n) is the exponent of 2 dividing n.) */
seq_t a(idx_t n)
{
    /* Return 0 for odd indices */
    if (n & 1) {
        return (seq_t)0;
    }

    /* Count the number of trailing zeros of n to obtain v2(n) */
    unsigned int tz = 0;
    idx_t t = n;
    while ((t & 1ULL) == 0) {
        t >>= 1;
        tz++;
    }

    /* Compute the value according to v2(n) mod 3 */
    unsigned int r = tz % 3;
    return (seq_t)(r == 0 ? 3 : r);
}
