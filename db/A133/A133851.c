/*
 * A133851  Sloping binary representation of powers of 4 (A000302), slope = -1
 *          .
 * https://oeis.org/A133851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 4^(n/3) if n % 3 == 0, otherwise 0
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 47, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Fast path for non‑multiples of 3 */
    if ((n % 3) != 0)
        return (seq_t)0;

    idx_t k = n / 3;          /* exponent for 4 */
    seq_t val = (seq_t)1;     /* start from 1 */

    /* Multiply by 4, k times.  This works for all supported seq_t types: */
    for (idx_t i = 0; i < k; ++i)
        val *= (seq_t)4;

    return val;
}
