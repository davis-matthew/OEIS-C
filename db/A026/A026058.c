/*
 * A026058  a(n) = (d(n)-r(n))/2, where d = A026057 and r is the periodic
 *          sequence with fundamental period (0,0,1,0).
 * https://oeis.org/A026058
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n^3 + 12*n^2 - 25*n)/6 - (n%4==2 ? 1 : 0)) / 2
 * Range:     u8 n <= 4, u16 n <= 36, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A026058 for n >= 4. */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 4; for smaller n the value is undefined.
       Returning 0 for n < 4 is harmless and satisfies the compiler. */
    if (n < 4)
        return 0;

    seq_t nn = (seq_t)n;           /* n converted to seq_t */
    seq_t n2 = nn * nn;           /* n^2     */
    seq_t n3 = n2 * nn;           /* n^3     */

    /* numerator of d(n) = (n^3 + 12 n^2 - 25 n) / 6 */
    seq_t num = n3 + (seq_t)12 * n2 - (seq_t)25 * nn;

    seq_t d = num / 6;            /* d(n) */

    /* subtract the periodic r(n) which is 1 exactly when n ≡ 2 (mod 4) */
    if ((n & 3) == 2)
        d -= 1;

    return d / 2;                 /* a(n) = (d - r) / 2 */
}
