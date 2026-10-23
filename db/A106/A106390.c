/*
 * A106390  Numbers k such that 13k = 6j^2 + 6j + 1.
 * https://oeis.org/A106390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; for n≥2: if n is odd: a(n)=a(n-1)+18*(n-1) if n is even:
 *            a(n)=a(n-1)+60*(n-1)
 * Range:     u8 n <= 3, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) is the first term, no definition below 1 */
    if (n == 0) return 0;

    seq_t result = 1;          /* a(1) */
    for (idx_t k = 2; k <= n; ++k) {
        seq_t mult = (k - 1);  /* (k-1) fits in seq_t for all tested n */
        seq_t inc;
        if (k & 1) {           /* k is odd */
            inc = (seq_t)18 * mult;
        } else {               /* k is even */
            inc = (seq_t)60 * mult;
        }
        result += inc;
    }
    return result;
}
