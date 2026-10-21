/*
 * A219628  Number of 2 X n arrays of the minimum value of corresponding
 *          elements and their horizontal or antidiagonal neighbors in a
 *          random, but sorted with lexicographically nondecreasing rows and
 *          columns, 0..2 2 X n array.
 * https://oeis.org/A219628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*n^4 - 22*n^3 + 189*n^2 - 290*n + 72)/24 for n>3
 *            Initial values: a(1)=6, a(2)=11, a(3)=24
 * Range:     u8 n <= 6, u16 n <= 28, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 6;
    if (n == 2) return 11;
    if (n == 3) return 24;

    /* use a 128‑bit intermediate type – safe up to n≈10 000 */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 n2 = nn * nn;
    unsigned __int128 n3 = n2 * nn;
    unsigned __int128 n4 = n3 * nn;

    unsigned __int128 num =
        (unsigned __int128)3   * n4
      - (unsigned __int128)22  * n3
      + (unsigned __int128)189 * n2
      - (unsigned __int128)290 * nn
      + (unsigned __int128)72;

    return (seq_t)(num / 24);
}
