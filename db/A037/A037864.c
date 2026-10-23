/*
 * A037864  a(n)=(number of digits <=2)-(number of digits >2) in base 5
 *          representation of n.
 * https://oeis.org/A037864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, sign
 * Formula:   a(n) = (# of digits ≤ 2 in base‑5 representation of n) minus (#
 *            of digits > 2)
 * Range:     i8 n <= 82, i16 n <= 82, i32 n <= 82, i64 n <= 82, i128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t less_eq2 = 0;
    seq_t greater2 = 0;
    idx_t x = n;

    while (x > 0) {
        idx_t digit = x % 5;
        if (digit <= 2)
            less_eq2++;
        else
            greater2++;
        x /= 5;
    }

    return less_eq2 - greater2;
}
