/*
 * A185906  Weight array of A000007 (which has only one nonzero term and whose
 *          second accumulation array is the multiplication table for the
 *          positive integers), by antidiagonals.
 * https://oeis.org/A185906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = (-1)^floor(n/2)*sign(floor(5/n)) - floor(n/4)*floor(4/n)
 * Range:     i8 n <= 105, i16 n <= 105, i32 n <= 105, i64 n <= 105,
 *            i128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 5) {
        /* (-1)^floor(n/2): parity of floor(n/2) = (n>>1)&1   */
        seq_t first = (( (n >> 1) & 1 ) ? -1 : 1);          /* sign(floor(5/n)) = 1 for n<=5 */
        /* Only n = 4 makes the second term non‑zero: -1*1*1 = -1 */
        seq_t second = (n == 4) ? -1 : 0;
        return first + second;
    }
    return 0; /* for n > 5 all terms are 0 */
}
