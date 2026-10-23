/*
 * A142590  First trisection of A061037 (Balmer line series of the hydrogen
 *          atom).
 * https://oeis.org/A142590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*(3*n+4)*(37 - 27*cos(n*Pi)-6*cos(n*Pi/2))/64
 *            Simplified to an integer formula: a(n) = 3*n*(3*n+4) if n mod 4
 *            == 1 or 3 a(n) = 3*n*(3*n+4)/16 if n mod 4 == 0 a(n) =
 *            3*n*(3*n+4)/4 if n mod 4 == 2
 * Range:     u8 n <= 4, u16 n <= 84, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn   = (seq_t)n;        /* Convert index to seq_t for arithmetic   */
    seq_t t    = 3 * tn * (3 * tn + 4); /* 3*n*(3*n+4)  */
    seq_t res;

    /* Decide how much to divide based on n mod 4 */
    switch (n & 3) {
        case 0:          /* factor = 4, divide by 16 */
            res = t / 16;
            break;
        case 1:
        case 3:          /* factor = 64, no division */
            res = t;
            break;
        case 2:          /* factor = 16, divide by 4 */
            res = t / 4;
            break;
    }
    return res;
}
