/*
 * A079795  Duplicate of A052037.
 * https://oeis.org/A079795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 1 for n <= 8; otherwise a(n) = 11 - (n mod 11), with the
 *            convention that 11 - 0 = 11. This matches the first 24 terms of
 *            A079795 (which is a duplicate of A052037)
 * Range:     i8 n <= 32, i16 n <= 32, i32 n <= 32, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* First eight terms are all 1 */
    if (n < 9)
        return (seq_t)1;

    /* Compute n mod 11.  Since n is unsigned, the modulo is well‑defined. */
    idx_t r = n % 11;

    /* If remainder is 0 we want the value 11, otherwise 11 - r. */
    if (r == 0)
        return (seq_t)11;
    else
        return (seq_t)(11 - r);
}
