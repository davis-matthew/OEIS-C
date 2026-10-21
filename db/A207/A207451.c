/*
 * A207451  Number of n X 6 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 26*n^4 + 78*n^3 - 104*n^2 + 26*n
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 112, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Index starts at 1; return 0 for n==0 to keep the function safe. */
    if (n == 0u)
        return 0;

    seq_t nn = (seq_t)n;           /* n */
    seq_t n2 = nn * nn;            /* n^2 */
    seq_t n3 = n2 * nn;            /* n^3 */
    seq_t n4 = n3 * nn;            /* n^4 */

    seq_t term1 = 26 * n4;         /* 26*n^4 */
    seq_t term2 = 78 * n3;         /* 78*n^3 */
    seq_t term3 = 104 * n2;        /* 104*n^2 */
    seq_t term4 = 26 * nn;         /* 26*n */

    seq_t result = term1 + term2 - term3 + term4;
    return result;
}
