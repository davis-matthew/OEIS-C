/*
 * A068250  1/24 the number of colorings of a 3 X 3 octagonal array with n
 *          colors.
 * https://oeis.org/A068250
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^9 -20*n^8 +174*n^7 -859*n^6 +2627*n^5 -5082*n^4
 *            +6048*n^3 -4023*n^2 +1134*n)/24 (valid for all n ≥ 4)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 18, u64 n <= 199, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute the numerator in signed 128‑bit to avoid overflow
       (n up to 10 000 gives n^9 ≈ 1e36, which fits in 128‑bit) */
    __int128_t N = n;
    __int128_t num = 1;          /* coefficient of n^9 */

    num = num * N - 20;          /* n^2 term   */
    num = num * N + 174;         /* n^3 term   */
    num = num * N - 859;         /* n^4 term   */
    num = num * N + 2627;        /* n^5 term   */
    num = num * N - 5082;        /* n^6 term   */
    num = num * N + 6048;        /* n^7 term   */
    num = num * N - 4023;        /* n^8 term   */
    num = num * N + 1134;        /* n^9 term   */
    num = num * N;               /* final *n   */

    num /= 24;                   /* exact division for n ≥ 4 */

    return (seq_t)num;
}
