/*
 * A121888  a(n) = (n^6 - 126*n^5 + 6217*n^4 - 153066*n^3 + 1987786*n^2 -
 *          13055316*n + 34747236)/36.
 * https://oeis.org/A121888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (n^6 - 126*n^5 + 6217*n^4 - 153066*n^3 + 1987786*n^2 -
 *            13055316*n + 34747236)/36
 * Range:     i8 none, i16 none, i32 n <= 87, i64 n <= 1000, i128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic for safe intermediate results.
       This works on GCC/Clang and is a no‑op when seq_t is 128‑bit.     */
    __int128 nn = n;            /* convert index to 128‑bit */
    __int128 n2 = nn * nn;
    __int128 n3 = n2 * nn;
    __int128 n4 = n3 * nn;
    __int128 n5 = n4 * nn;
    __int128 n6 = n5 * nn;

    __int128 numerator =
        n6
        - (__int128)126 * n5
        + (__int128)6217 * n4
        - (__int128)153066 * n3
        + (__int128)1987786 * n2
        - (__int128)13055316 * nn
        + 34747236;

    numerator /= 36;            /* exact division for all integer n */

    return (seq_t)numerator;    /* truncate if seq_t is smaller than 128‑bit */
}
