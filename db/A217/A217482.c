/*
 * A217482  Quarter-square tetrahedrals: a(n) = k*(k - 1)*(k - 2)/6, k =
 *          A002620(n).
 * https://oeis.org/A217482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = k*(k-1)*(k-2)/6, where k = floor(n^2/4)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4)          /* for n = 0,1,2,3 the term is 0 */
        return 0;

    seq_t n_seq = n;
    seq_t half   = n_seq / 2;            /* floor(n/2) */
    seq_t k;

    if ((n & 1) == 0)                   /* n even: k = (n/2)^2           */
        k = half * half;
    else                                /* n odd : k = (n/2)*(n/2+1)     */
        k = half * (half + 1);

    seq_t num = k * (k - 1) * (k - 2);   /* k*(k-1)*(k-2)               */
    return num / 6;                      /* division is exact           */
}
