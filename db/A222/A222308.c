/*
 * A222308  Let P be a one-move "rider" with move set M={(1,2)}; a(n) is the
 *          number of non-attacking positions of two indistinguishable pieces
 *          P on an n X n board.
 * https://oeis.org/A222308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4/2 - 5*n^3/24 - 11*n/48 + (-1)^n*n/16
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The OEIS offset is 1; for n=0 we return 0 although it is never queried. */
    if (n == 0ULL) return 0;

    long long nn = (long long)n;          /* use signed 64‑bit for intermediate */
    long long n2 = nn * nn;               /* n^2 */
    long long n3 = n2 * nn;               /* n^3 */
    long long n4 = n3 * nn;               /* n^4 */

    long long num = 24LL * n4              /* 24*n^4  */
                  - 10LL * n3             /* -10*n^3 */
                  - 11LL * nn;            /* -11*n   */

    /* add or subtract 3*n depending on the parity of n */
    if ((nn & 1LL) == 0) {
        num += 3LL * nn;                   /* even n : + 3*n */
    } else {
        num -= 3LL * nn;                   /* odd  n : - 3*n */
    }

    /* division by 48 is exact for all n ≥ 1 */
    return (seq_t)(num / 48LL);
}
