/*
 * A084633  Inverse binomial transform of repeated odd numbers.
 * https://oeis.org/A084633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 63, i128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 1                 (n = 0)
 *            0                 (n = 1)
 *            (-1)^n * 2^(n-1)   (n > 1)
 * The implementation uses repeated multiplication by 2 to avoid
 * undefined behaviour from right‑shifting built‑in integer types
 * beyond their natural width.  It works for any integer type
 * (int8_t … int128) as well as for an arbitrary‑precision
 * big‑integer class that supports the usual arithmetic operators.
 */
seq_t a(idx_t n)
{
    /* Handle the two explicit initial values. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)0;

    /* Compute 2^(n-1) by repeated multiplication. */
    seq_t power = (seq_t)1;
    for (idx_t i = 1; i < n; ++i)
        power *= (seq_t)2;   /* multiply by 2, (n-1) times */

    /* Apply the alternating sign (-1)^n. */
    if (n & 1)   /* odd n => negative sign */
        power = -power;

    return power;
}
