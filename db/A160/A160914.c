/*
 * A160914  Extended s-block elements for Janet table.
 * https://oeis.org/A160914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(2k-1) = 1 + Σ_{i=1}^{k-1} 2 * ((i/2 + 1)^2), a(2k) = a(2k-1) +
 *            1 (i/2 denotes integer division)
 * Range:     u8 n <= 20, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* out of range, not used */
    idx_t k = (n + 1) >> 1;            /* ceil(n/2), index of the odd term */
    seq_t val = 1;                     /* b₁ = 1 */

    for (idx_t i = 1; i < k; ++i) {
        idx_t step = i / 2 + 1;         /* floor(i/2)+1   */
        seq_t s = (seq_t)step;
        val += s * s * 2;               /* add 2*step² */
    }

    return (n & 1) ? val : val + 1;    /* even indices get +1 */
}
