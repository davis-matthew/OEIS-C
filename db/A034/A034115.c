/*
 * A034115  Fractional part of square root of a(n) starts with 9: first term
 *          of runs.
 * https://oeis.org/A034115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n^2 + 9*n + 25 + floor(4*n/5)
 * Range:     u8 n <= 11, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = nn * nn + (seq_t)9 * nn + (seq_t)25
                + ((seq_t)4 * nn) / (seq_t)5;
    return val;
}
