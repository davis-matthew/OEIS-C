/*
 * A132151  Period 8: repeat [0, 1, 0, 0, 0, 0, -1, 0].
 * https://oeis.org/A132151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = floor((n-1)/8) - floor((n-2)/8) - floor((n-6)/8) +
 *            floor((n-7)/8)
 * Range:     i8 n <= 87, i16 n <= 87, i32 n <= 87, i64 n <= 87, i128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic pattern of length 8: [0, 1, 0, 0, 0, 0, -1, 0] */

seq_t a(idx_t n)
{
    /* Compute n modulo 8 using bitwise AND (works because 8 is a power of two). */
    idx_t r = n & 7;

    switch (r) {
        case 1:
            return 1;   /* a(1) =  1, a(9) =  1, ... */
        case 6:
            return -1;  /* a(6) = -1, a(14) = -1, ... */
        default:
            return 0;   /* all other residues yield 0 */
    }
}
