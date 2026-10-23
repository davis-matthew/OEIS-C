/*
 * A029013  Expansion of 1/((1-x)*(1-x^2)*(1-x^5)*(1-x^8)).
 * https://oeis.org/A029013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n^3 + 48*n^2 + (337 + 15*(-1)^n)*n + 1008)/960)
 *            + [(n mod 40) = 10] (Number of partitions of n into parts 1, 2,
 *            5 and 8.)
 * Range:     u8 n <= 41, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use signed 64‑bit arithmetic for the intermediate polynomial */
    long long nn = (long long)n;
    long long num =
        2LL * nn * nn * nn                 /* 2*n^3 */
        + 48LL * nn * nn                   /* 48*n^2 */
        + (337 + 15LL * ((nn & 1LL) ? -1LL : 1LL)) * nn /* (337+15*(-1)^n)*n */
        + 1008LL;                          /* +1008 */

    seq_t result = (seq_t)(num / 960LL);   /* floor division, positive */

    if ((n % 40) == 10)
        result += 1;                      /* add the indicator term */

    return result;
}
