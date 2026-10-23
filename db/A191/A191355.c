/*
 * A191355  Indices of terms in A069748 with two decimal digits 1 and all
 *          others 0.
 * https://oeis.org/A191355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = -(-1)^n/8 + A049347(n)/3 + 115/24 + n/6 + n^2/4 where
 *            A049347(n) = 1 if n mod 3 == 0 -1 if n mod 3 == 1 0 if n mod 3
 *            == 2
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1; guard against 0 */

    /* (-1)^n :  1 for even n, -1 for odd n */
    long long s = ((n & 1) == 0) ? 1LL : -1LL;

    /* A049347(n) pattern */
    int r = n % 3;
    long long val = (r == 0) ? 1LL : (r == 1) ? -1LL : 0LL;

    /* Compute the numerator of the closed‑form expression */
    long long nn = (long long)n;
    long long numerator =
        -3LL * s            /* -(-1)^n * 3    */
        + 8LL * val         /* A049347(n) * 8  */
        + 115LL             /* constant      */
        + 4LL * nn          /* n/6 part      */
        + 6LL * nn * nn;    /* n^2/4 part    */

    return (seq_t)(numerator / 24LL);
}
