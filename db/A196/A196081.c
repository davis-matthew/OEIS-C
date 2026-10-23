/*
 * A196081  Dungeons and Dragons Ability Modifier Sequence.
 * https://oeis.org/A196081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = n + 10 a(2n+1) = floor(n / 2) (equivalent recurrence:
 *            a(n+5) = a(n) + a(n+1) - a(n+4) + 3)
 * Range:     u8 n <= 321, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1u) == 0) {
        return (((seq_t)n) / 2u) + (seq_t)10;
    } else {
        return ((seq_t)(n - 1u)) / 4u;
    }
}
