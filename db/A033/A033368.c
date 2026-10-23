/*
 * A033368  a(n) = floor(48/n).
 * https://oeis.org/A033368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ⌊48/n⌋ (offset 1)
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; behaviour for n = 0 is irrelevant. */
    if (n == 0)
        return 0;
    /* Compute the floor of 48/n using integer division. */
    return (seq_t)48 / (seq_t)n;
}
