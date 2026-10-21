/*
 * A097252  Numbers whose set of base 6 digits is {0,5}.
 * https://oeis.org/A097252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(2n)=6*a(n), a(2n+1)=a(2n)+5, a(0)=0 (numbers whose base‑6
 *            digits are only 0 or 5)
 * Range:     u8 n <= 7, u16 n <= 63, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res   = 0;     /* accumulated value */
    seq_t power = 5;     /* 5 * 6^0, the contribution when a bit is 1 */

    idx_t m = n;         /* work on a copy of n */

    while (m) {
        if (m & 1u)
            res += power;   /* add 5*6^i when the i‑th bit is set */

        power *= 6;        /* move to the next power of 6 */
        m >>= 1;           /* shift right to examine next bit */
    }

    return res;
}
