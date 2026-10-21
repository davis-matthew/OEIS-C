/*
 * A219775  Number of 4Xn arrays of the minimum value of corresponding
 *          elements and their horizontal or antidiagonal neighbors in a
 *          random, but sorted with lexicographically nondecreasing rows and
 *          columns, 0..1 4Xn array.
 * https://oeis.org/A219775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 50, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A219775
 * Formula: a(n) = (n^8 + 6n^7 - 210n^6 + 3444n^5 - 11991n^4 - 24906n^3
 *              + 778280n^2 - 6197904n + 17488800) / 10080   for n > 9
 *          a(1)=5, a(2)=8, a(3)=29, a(4)=100, a(5)=291, a(6)=771,
 *          a(7)=1887, a(8)=4278, a(9)=9072                                           */

static const seq_t FIRST[9] = {
    5, 8, 29, 100, 291, 771, 1887, 4278, 9072
};

seq_t a(idx_t n)
{
    if (n <= 9)
        return FIRST[n - 1];

    /* Use 128‑bit integer for intermediate calculations. */
    __int128 NN = (__int128)n;

    /* Compute the numerator of the polynomial in 128‑bit. */
    __int128 num = NN * NN * NN * NN * NN * NN * NN * NN;            /* n^8 */
    num += (__int128)6 * NN * NN * NN * NN * NN * NN * NN;           /* +6 n^7 */
    num -= (__int128)210 * NN * NN * NN * NN * NN * NN;              /* -210 n^6 */
    num += (__int128)3444 * NN * NN * NN * NN * NN;                 /* +3444 n^5 */
    num -= (__int128)11991 * NN * NN * NN * NN;                     /* -11991 n^4 */
    num -= (__int128)24906 * NN * NN * NN;                          /* -24906 n^3 */
    num += (__int128)778280 * NN * NN;                              /* +778280 n^2 */
    num -= (__int128)6197904 * NN;                                  /* -6197904 n */
    num += (__int128)17488800;                                      /* +17488800 */

    /* Divide by 10080 to obtain the exact result. */
    return (seq_t)(num / (__int128)10080);
}
