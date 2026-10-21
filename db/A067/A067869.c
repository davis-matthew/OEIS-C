/*
 * A067869  Numbers n such that n and 2^n end with the same 5 digits.
 * https://oeis.org/A067869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 48736 + 100000*(n-1)
 * Range:     u8 none, u16 n <= 1, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) for n ≥ 1, with a(1) = 48736 */
seq_t a(idx_t n)
{
    seq_t base = 48736;
    seq_t diff = 100000;
    return base + diff * ((seq_t)(n - 1));
}
