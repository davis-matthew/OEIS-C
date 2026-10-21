/*
 * A062880  Zero together with the numbers which can be written as a sum of
 *          distinct odd powers of 2.
 * https://oeis.org/A062880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * Σ_{i≥0} 4^i * bit_i(n) (i.e. a(n) is 2 times the
 *            number obtained by interpreting the binary representation of n
 *            as a base‑4 number)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t pow4   = 1;          /* 4^i, starting with i = 0 */

    while (n)
    {
        if (n & 1)            /* if the i‑th bit of n is 1 */
            result += pow4 << 1;   /* add 2·4^i */

        pow4 <<= 2;           /* next power of 4 (×4) */
        n >>= 1;              /* move to the next bit of n */
    }

    return result;
}
