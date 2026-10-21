/*
 * A093392  [n/25] + [(n+1)/25] + [(n+2)/25] + [(n+3)/25] + [(n+4)/25].
 * https://oeis.org/A093392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊n/25⌋ + ⌊(n+1)/25⌋ + ⌊(n+2)/25⌋ + ⌊(n+3)/25⌋ +
 *            ⌊(n+4)/25⌋
 * Range:     u8 n <= 1294, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute each term in seq_t arithmetic to respect overflow limits. */
    seq_t v0 = ((seq_t)n)/25;
    seq_t v1 = ((seq_t)(n+1))/25;
    seq_t v2 = ((seq_t)(n+2))/25;
    seq_t v3 = ((seq_t)(n+3))/25;
    seq_t v4 = ((seq_t)(n+4))/25;
    return v0 + v1 + v2 + v3 + v4;
}
