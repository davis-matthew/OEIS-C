/*
 * A040504  Continued fraction for sqrt(527).
 * https://oeis.org/A040504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less, mult
 * Formula:   multiplicative sequence a(0) = 22 a(1) = 1 a(2) = 21 a(2^e) = 44
 *            for e >= 2 a(p^e) = 1 for any odd prime p and e >= 1 (The value
 *            depends only on the 2‑adic valuation of n.)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the offset term */
    if (n == (idx_t)0)
        return (seq_t)22;

    /* handle n = 1 */
    if (n == (idx_t)1)
        return (seq_t)1;

    /* compute exponent of 2 in n */
    idx_t e = 0;
    idx_t t = n;
    while ((t & (idx_t)1) == (idx_t)0) {
        e++;
        t >>= 1;
    }

    /* multiplicative value depends only on e */
    if (e == (idx_t)0)
        return (seq_t)1;        /* odd n > 1 */
    if (e == (idx_t)1)
        return (seq_t)21;       /* n even but not divisible by 4 */
    return (seq_t)44;           /* n divisible by 4 */
}
