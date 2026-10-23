/*
 * A102815  "False so far" sequence.
 * https://oeis.org/A102815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(1) = 11; a(n) = n + 18 for 2 <= n <= 81; a(n) = n + 19 for n
 *            >= 82
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)11;
    if (n <= (idx_t)81)
        return (seq_t)n + (seq_t)18;
    return (seq_t)n + (seq_t)19;
}
