/*
 * A218470  Partial sums of floor(n/9).
 * https://oeis.org/A218470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = sum_{k=0}^n ⌊k/9⌋ Equivalent closed form: a(n) = 9 *
 *            ⌊n/9⌋ * (⌊n/9⌋ - 1) / 2 + (n % 9 + 1) * ⌊n/9⌋
 * Range:     u8 n <= 71, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t q = n / 9;          /* ⌊n/9⌋ */
    idx_t r = n % 9;          /* remainder n mod 9   */
    seq_t q_seq = (seq_t)q;   /* convert to seq_t     */

    seq_t part1 = ((seq_t)9) * q_seq * (q_seq - 1) / 2;
    seq_t part2 = q_seq * (((seq_t)r) + 1);

    return part1 + part2;
}
