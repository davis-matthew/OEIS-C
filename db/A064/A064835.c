/*
 * A064835  If n mod 2 = 0 then a(n) = n^4/4 - 2*n^2 + 3*n; otherwise, a(n) =
 *          n^4/4 - 2*n^2 + 3*n - 5/4.
 * https://oeis.org/A064835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*m^4 - 8*m^2 + 6*m when n is even, m = n/2 a(n) = 4*m^4
 *            + 8*m^3 - 2*m^2 when n is odd, m = (n-1)/2 This is an integer
 *            form of the OEIS definition: a(n) = n^4/4 - 2*n^2 + 3*n if n is
 *            even a(n) = n^4/4 - 2*n^2 + 3*n - 5/4 if n is odd
 * Range:     u8 n <= 5, u16 n <= 22, u32 n <= 362, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n = 0 or 1 gives 0 by the formula */
    if ((n & 1) == 0) {
        seq_t m  = (seq_t)n / 2;      /* m = n/2 */
        seq_t m2 = m * m;             /* m² */
        seq_t m4 = m2 * m2;           /* m⁴ */
        return 4 * m4 - 8 * m2 + 6 * m; /* 4m⁴ – 8m² + 6m */
    } else {
        seq_t m  = ((seq_t)n - 1) / 2;/* m = (n-1)/2 */
        seq_t m2 = m * m;             /* m² */
        seq_t m3 = m2 * m;            /* m³ */
        seq_t m4 = m2 * m2;           /* m⁴ */
        return 4 * m4 + 8 * m3 - 2 * m2; /* 4m⁴ + 8m³ – 2m² */
    }
}
