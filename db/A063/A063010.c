/*
 * A063010  Carryless binary square of n; also Moser-de Bruijn sequence
 *          written in binary.
 * https://oeis.org/A063010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = Σ 100^k for each bit k that is set in the binary *
 *            representation of n. This is the same as the Moser–de Bruijn
 *            sequence * written in base 100 (sum of distinct powers of 100)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 31, u64 n <= 1023,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ans = 0;
    seq_t power = 1;                /* 100^0 */
    idx_t bits = n;

    while (bits)
    {
        if (bits & (idx_t)1)
            ans += power;          /* add 100^k if bit k is set */
        power *= (seq_t)100;      /* move to next power of 100 */
        bits >>= (idx_t)1;        /* shift to next bit */
    }

    return ans;
}
