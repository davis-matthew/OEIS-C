/*
 * A180004  a(n) is the nearest integer to n*(27/26).
 * https://oeis.org/A180004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor((27*n + 13)/26) (nearest integer to n*(27/26))
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use 128‑bit arithmetic to avoid overflow of 27*n for large n */
    return (seq_t)(((unsigned __int128)n * 27 + 13) / 26);
}
