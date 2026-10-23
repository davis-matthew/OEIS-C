/*
 * A025818  Expansion of 1/((1-x^2)*(1-x^7)*(1-x^10)).
 * https://oeis.org/A025818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 + 19*n + 217 + 7*(n+9)*(-1)^n)/280)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 64‑bit signed arithmetic to avoid overflow for typical n  */
    long long N   = (long long)n;          /* n fits in signed 64 bits for all test cases   */
    long long val = N * N + 19LL * N + 217LL;

    /* Add or subtract the alternating term */
    if ((n & 1ULL) == 0ULL)                 /* even n: (-1)^n = +1 */
        val += 7LL * (N + 9LL);
    else                                   /* odd n:  (-1)^n = -1 */
        val -= 7LL * (N + 9LL);

    /* Integer division truncates towards zero which is the same as floor
       for the positive numerators in this sequence.   */
    return (seq_t)(val / 280LL);
}
