/*
 * A040693  Continued fraction for sqrt(720).
 * https://oeis.org/A040693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0) = 26; for n > 0: a(n) = 1 if n is odd 4 if 2‖n (exactly one
 *            factor 2) 52 if 2²|n (at least two factors of 2)
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)26;

    /* count the exponent of 2 in n */
    uint64_t m = (uint64_t)n;
    int e = 0;
    while ((m & 1ULL) == 0) {
        e++;
        m >>= 1;
    }

    if (e == 0)          /* odd */
        return (seq_t)1;
    else if (e == 1)     /* exactly one 2 */
        return (seq_t)4;
    else                 /* two or more 2's */
        return (seq_t)52;
}
