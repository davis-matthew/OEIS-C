/*
 * A216430  (-1)^A081603(n), parity of the number of 2's in the ternary
 *          expansion of n.
 * https://oeis.org/A216430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, base
 * Formula:   a(n) = (-1)^(number of 2's in base‑3 representation of n) a(n) =
 *            1 if the count of 2's in the ternary expansion of n is even, -1
 *            otherwise
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n;
    unsigned cnt = 0;
    const idx_t three = (idx_t)3;
    const idx_t two   = (idx_t)2;
    while (m) {
        if ((m % three) == two) {
            cnt++;
        }
        m /= three;
    }
    return (seq_t)((cnt & 1) ? -1 : 1);
}
