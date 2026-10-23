/*
 * A168189  a(n) = n^3*(n^8 + 1)/2.
 * https://oeis.org/A168189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^3*(n^8 + 1)/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 60, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    if (sn == 0)
        return 0;

    seq_t n2 = sn * sn;            /* n^2   */
    seq_t n4 = n2 * n2;            /* n^4   */
    seq_t n8 = n4 * n4;            /* n^8   */

    seq_t ans;
    if ((sn & 1) == 0) {           /* n even → divide n^3 by 2 first */
        seq_t half = sn / 2;       /* n/2   */
        seq_t n3_half = half * n2; /* (n/2)*n^2 = n^3/2 */
        ans = n3_half * (n8 + 1);
    } else {                      /* n odd  → divide (n^8+1) by 2 first */
        seq_t half = (n8 + 1) / 2;
        seq_t n3 = n2 * sn;       /* n^3   */
        ans = half * n3;
    }
    return ans;
}
