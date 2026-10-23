/*
 * A143371  Duplicate of A131423.
 * https://oeis.org/A143371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n*(n+1)*(2*n+1)/3 - n
 * Range:     i8 n <= 5, i16 n <= 36, i32 n <= 41, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a 128‑bit intermediate for safety */
    __int128 nn = (__int128)n;
    __int128 prod = nn * (nn + 1) * (2 * nn + 1);
    prod /= 3;
    prod -= nn;
    return (seq_t)prod;
}
