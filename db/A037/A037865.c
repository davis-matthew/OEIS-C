/*
 * A037865  a(n)=(number of digits <=2)-(number of digits >2) in base 6
 *          representation of n.
 * https://oeis.org/A037865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, sign
 * Formula:   a(n) = (# digits ≤ 2) – (# digits > 2) in the base‑6
 *            representation of n
 * Range:     i8 n <= 81, i16 n <= 81, i32 n <= 81, i64 n <= 81, i128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t diff = 0;
    idx_t m = n;
    while (m > 0) {
        idx_t digit = m % 6;
        if (digit <= 2)
            diff += 1;
        else
            diff -= 1;
        m /= 6;
    }
    return diff;
}
