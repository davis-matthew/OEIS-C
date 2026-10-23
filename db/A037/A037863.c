/*
 * A037863  a(n) = (number of digits <=1) - (number of digits >1) in base 4
 *          representation of n.
 * https://oeis.org/A037863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, sign
 * Formula:   a(n) = (#digits ≤ 1 in base‑4 representation of n) − (#digits >
 *            1 in base‑4 representation of n)
 * Range:     i8 n <= 75, i16 n <= 75, i32 n <= 75, i64 n <= 75, i128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t diff = 0;     /* difference between low and high base‑4 digits */
    idx_t m = n;        /* copy of n for manipulation without affecting the caller */

    while (m > 0) {                 /* iterate over base‑4 digits */
        idx_t digit = m & 3;        /* low two bits give the current digit (0–3) */
        if (digit <= 1)
            diff++;                 /* count of digits ≤ 1 */
        else
            diff--;                 /* count of digits > 1 */
        m >>= 2;                    /* shift right by 2 bits → divide by 4 */
    }
    return diff;
}
