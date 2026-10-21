/*
 * A230074  Period 4: repeat [1, -2, 1, 0].
 * https://oeis.org/A230074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = 1 if n mod 4 == 1 or 3; = -2 if n mod 4 == 2; = 0 if n
 *            mod 4 == 0 (period 4)
 * Range:     i8 n <= 110, i16 n <= 110, i32 n <= 110, i64 n <= 110,
 *            i128 n <= 110, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Since n >= 1, only the remainders 1,2,3,0 matter. */
    switch ((int)(n & 3))
    {
        case 1:
            return (seq_t)1;      /* n ≡ 1 (mod 4) */
        case 2:
            return (seq_t)-2;     /* n ≡ 2 (mod 4) */
        case 3:
            return (seq_t)1;      /* n ≡ 3 (mod 4) */
        default:                 /* n ≡ 0 (mod 4) */
            return (seq_t)0;      /* n ≡ 0 (mod 4) */
    }
}
