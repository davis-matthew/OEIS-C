/*
 * A080340  First known infinite sequence containing no odd integer of the
 *          form 2^m+p (p prime).
 * https://oeis.org/A080340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*11184810 + 7629217
 * Range:     u8 none, u16 none, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 0  (a(0) = 7629217) */
seq_t a(idx_t n)
{
    return (seq_t)n * (seq_t)11184810 + (seq_t)7629217;
}
