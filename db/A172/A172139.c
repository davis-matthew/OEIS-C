/*
 * A172139  Number of ways to place 4 nonattacking zebras on an n X n board.
 * https://oeis.org/A172139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^8 - 54*n^6 + 240*n^5 + 827*n^4 - 8592*n^3 + 10362*n^2
 *            + 75600*n - 204864)/24, valid for n >= 9; for smaller n the
 *            first 8 terms are used directly
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 24, u64 n <= 380, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* First 8 terms (offset 1).  a(1)=0 is the sequence offset. */
    static const seq_t small[9] = {0, 0, 1, 126, 1168, 7334, 35749, 137970, 438984};

    if (n < 9) {
        return small[n];
    }

    /* For n >= 9, compute the polynomial using a 128‑bit intermediate
       to avoid overflow on 64‑bit seq_t builds.  The calculation is
       performed with unsigned __int128, which is supported by GCC
       and Clang and compiles as a C++17 extension.  The result is
       guaranteed to be integral and fits in seq_t when seq_t can
       represent it. */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 n2 = nn * nn;          /* n^2   */
    unsigned __int128 n3 = n2 * nn;          /* n^3   */
    unsigned __int128 n4 = n2 * n2;          /* n^4   */
    unsigned __int128 n5 = n4 * nn;          /* n^5   */
    unsigned __int128 n6 = n3 * n3;          /* n^6   */
    unsigned __int128 n8 = n4 * n4;          /* n^8   */

    unsigned __int128 term = n8;
    term -= (unsigned __int128)54   * n6;
    term += (unsigned __int128)240  * n5;
    term += (unsigned __int128)827  * n4;
    term -= (unsigned __int128)8592 * n3;
    term += (unsigned __int128)10362 * n2;
    term += (unsigned __int128)75600 * nn;
    term -= (unsigned __int128)204864;

    term /= 24;

    return (seq_t)term;
}
