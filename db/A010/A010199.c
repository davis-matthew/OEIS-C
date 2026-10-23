/*
 * A010199  Continued fraction for sqrt(140).
 * https://oeis.org/A010199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(n) = 11 if n=0; 1 if n odd; 4 if n=2; 22 if v2(n)≥2
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)11;
    if (n == 1) return (seq_t)1;

    idx_t e = 0;
    idx_t t = n;
    while ((t & 1) == 0) {
        e++;
        t >>= 1;
    }
    if (e == 0) return (seq_t)1;   /* odd n */
    if (e == 1) return (seq_t)4;   /* n = 2 * odd */
    return (seq_t)22;              /* n divisible by 4 (any odd part contributes 1) */
}
