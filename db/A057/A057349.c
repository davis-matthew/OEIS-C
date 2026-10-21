/*
 * A057349  Leap years in the Hebrew Calendar starting in year 1 (3761 BCE).
 *          The leap year has an extra month.
 * https://oeis.org/A057349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((19*n + 5)/7)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)19 * n + (seq_t)5) / (seq_t)7;
}
