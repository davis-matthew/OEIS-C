/*
 * A166783  Number of nX3 1..4 arrays containing at least one of each value,
 *          all equal values connected, and rows considered as a single number
 *          in nondecreasing order.
 * https://oeis.org/A166783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/45360)*(n-1)*(226800 - 555696*n + 563760*n^2 +
 *            340780*n^3 + 306823*n^4 + 52219*n^5 + 10513*n^6 + 553*n^7 +
 *            40*n^8)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 5, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if (nn <= 1)   /* a(1)=0 */
        return 0;

    /* pre‑compute n^k for k = 1 … 8 */
    seq_t pow[9];
    pow[0] = 1;
    for (idx_t i = 1; i <= 8; ++i)
        pow[i] = pow[i-1] * nn;

    /* compute the polynomial inside the parentheses */
    seq_t poly = (seq_t)226800;
    poly += (seq_t)563760 * pow[2];
    poly += (seq_t)340780 * pow[3];
    poly += (seq_t)306823 * pow[4];
    poly += (seq_t)52219  * pow[5];
    poly += (seq_t)10513  * pow[6];
    poly += (seq_t)553    * pow[7];
    poly += (seq_t)40     * pow[8];
    poly -= (seq_t)555696 * pow[1];

    /* multiply by (n-1) and divide by 45360 */
    seq_t num = (nn - 1) * poly;
    return num / 45360;
}
