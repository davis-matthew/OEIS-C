/*
 * A160787  G.f.: (21+104*x+103*x^2+23*x^3+x^4)/(1-x)^5.
 * https://oeis.org/A160787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (63*n^4 + 247*n^3 + 441*n^2 + 377*n + 126) / 6
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 139, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 0  */
seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;

    /* Compute powers of n */
    seq_t n2 = sn * sn;            /* n^2   */
    seq_t n3 = n2 * sn;            /* n^3   */
    seq_t n4 = n3 * sn;            /* n^4   */

    /* Build the numerator of the rational expression */
    seq_t num = 63 * n4;           /* 63 n^4 */
    num += 247 * n3;               /* +247 n^3 */
    num += 441 * n2;               /* +441 n^2 */
    num += 377 * sn;               /* +377 n   */
    num += 126;                    /* +126     */

    /* All terms are divisible by 6; perform the final division */
    return num / 6;
}
