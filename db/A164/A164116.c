/*
 * A164116  Expansion of (1 - x) * (1 - x^4) / (1 - x^5) in powers of x.
 * https://oeis.org/A164116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   if n == 0: 1; else if n % 5 == 0: 2; else if n % 5 == 1 or 4:
 *            -1; else 0
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned rem = (unsigned)(n % 5);

    /* handle the special case n == 0 */
    if (n == 0)
        return (seq_t)1;

    switch (rem)
    {
        case 0:          /* n ≡ 0 (mod 5) and n > 0 */
            return (seq_t)2;
        case 1:          /* n ≡ 1 (mod 5) */
        case 4:          /* n ≡ 4 (mod 5) */
            return (seq_t)(-1);
        default:         /* n ≡ 2 or 3 (mod 5) */
            return (seq_t)0;
    }
}
