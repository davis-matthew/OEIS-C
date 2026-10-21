/*
 * A202270  a(n) is the largest n-digit number whose sum of digits is n.
 * https://oeis.org/A202270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   * a(n) = largest n‑digit decimal number whose digits sum to n. *
 *            Construction: write floor(n/9) digits '9', followed by digit n%9
 *            * (if non‑zero), then fill the remaining positions with zeros
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* number of 9’s */
    idx_t q = n / 9;
    /* remainder digit */
    idx_t r = n % 9;

    /* build the prefix consisting of all non‑zero digits */
    seq_t result = 0;
    for (idx_t i = 0; i < q; ++i)
        result = result * 10 + 9;           /* append a 9 */

    if (r != 0)
        result = result * 10 + r;           /* append the remainder */

    /* append the remaining zeros */
    idx_t zeros = n - q - (r != 0 ? 1 : 0);
    for (idx_t i = 0; i < zeros; ++i)
        result = result * 10;               /* multiply by 10 (add a zero) */

    return result;
}
