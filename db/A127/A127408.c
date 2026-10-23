/*
 * A127408  Negative value of coefficient of x^(n-3) in the characteristic
 *          polynomial of a certain n X n integer circulant matrix.
 * https://oeis.org/A127408
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6 + 2*n^5 - 13*n^4 + 10*n^3)/24 (n >= 3)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* values below the offset are not required by the tests */
    if (n < 3)
        return 0;

    /* use 128‑bit arithmetic for the intermediate computation */
    __int128 N = n;
    __int128 cube   = N * N * N;                  /* n^3 */
    __int128 poly   = N * N * N + 2 * N * N - 13 * N + 10; /* n^3 + 2n^2 -13n +10 */
    __int128 num    = cube * poly;                  /* n^6 + 2n^5 -13n^4 +10n^3 */
    __int128 result = num / 24;                      /* divide by 4! = 24 */

    return (seq_t)result;    /* implicit conversion works for integers and most big‑int types */
}
