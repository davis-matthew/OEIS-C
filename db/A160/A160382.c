/*
 * A160382  Number of 2's in base-4 representation of n.
 * https://oeis.org/A160382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   number of 2's in the base‑4 representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;          /* running count of digit '2' */
    while (n > 0) {         /* while there are still base‑4 digits left */
        if ((n & 3) == 2)   /* test the least significant 2 bits (a base‑4 digit) */
            res++;          /* it's a '2' */
        n >>= 2;            /* drop that base‑4 digit */
    }
    return res;             /* result fits in seq_t */
}
