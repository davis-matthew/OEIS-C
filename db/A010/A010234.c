/*
 * A010234  Continued fraction for sqrt(192).
 * https://oeis.org/A010234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=13; otherwise a(n)=1 if n is odd, a(n)=5 if n≡2 (mod 4),
 *            a(n)=26 if n≡0 (mod 4)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)           /* first term */
        return (seq_t)13;
    if (n == (idx_t)1)           /* second term is 1 */
        return (seq_t)1;

    /* factor out powers of 2 from n */
    idx_t m = n;
    unsigned e2 = 0;
    while ((m & (idx_t)1) == (idx_t)0) {
        e2++;
        m >>= 1;
    }

    /* results depend only on the exponent of 2 */
    if (e2 == 0) return (seq_t)1;        /* n odd */
    if (e2 == 1) return (seq_t)5;        /* n ≡ 2 (mod 4) */
    return (seq_t)26;                    /* n ≡ 0 (mod 4) with at least two 2‑factors */
}
