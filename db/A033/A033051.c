/*
 * A033051  Numbers whose set of base 15 digits is {0,1}.
 * https://oeis.org/A033051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = ∑ ((n >> i) & 1) * 15^i, i = 0..∞
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 511, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;   /* the computed value */
    seq_t power  = 1;   /* 15^i, starting with i = 0 */
    idx_t temp = n;     /* work on a copy of n */

    while (temp) {
        if (temp & (idx_t)1)        /* if the i‑th bit of n is set */
            result += power;      /* add 15^i to the result */

        power *= (seq_t)15;        /* advance to the next power of 15 */
        temp >>= 1;                /* move to the next bit */
    }

    return result;
}
