/*
 * A172230  Number of ways to place 4 nonattacking wazirs on a 4 X n board.
 * https://oeis.org/A172230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 (n=1), 2 (n=2), (64*n^4 - 432*n^3 + 1235*n^2 - 1797*n +
 *            1122)/6 (n≥3)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 143, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)2;

    /* Use a wider signed type for intermediate arithmetic;
       the result for n≤~10000 fits in 64 bits. */
    long long t = (long long)n;

    long long numer =
        64LL * t * t * t * t
      - 432LL * t * t * t
      + 1235LL * t * t
      - 1797LL * t
      + 1122LL;

    /* Integer division by 6; sequence values are always divisible by 6. */
    seq_t result = (seq_t)(numer / 6LL);
    return result;
}
