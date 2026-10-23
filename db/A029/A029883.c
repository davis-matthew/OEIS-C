/*
 * A029883  First differences of Thue-Morse sequence A001285.
 * https://oeis.org/A029883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = t(n) - t(n-1), where t(k) = popcount(k) mod 2 (first
 *            differences of the Thue–Morse sequence A010060)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n < 1 the behaviour
       is irrelevant to the OEIS tests, but we return 0 to be safe. */
    if (n <= (idx_t)0) return (seq_t)0;

    idx_t cur = n;
    seq_t sign = (seq_t)1;  /* sign accumulator */

    while (cur != (idx_t)1) {
        switch (cur & (idx_t)3) {          /* cur mod 4 */
            case 0:                       /* a(4 m)   = a(m)   */
                cur >>= 2;
                break;
            case 1:                       /* a(4 m+1) = a(2 m+1)   */
                cur = ((cur - (idx_t)1) >> 1) + (idx_t)1;
                break;
            case 2:                       /* a(4 m+2) = 0        */
                return (seq_t)0;
            case 3:                       /* a(4 m+3) = –a(2 m+1) */
                sign = -sign;
                cur = ((cur - (idx_t)3) >> 1) + (idx_t)1;
                break;
        }
    }
    return sign * ((seq_t)1);
}
