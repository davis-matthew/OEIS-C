/*
 * A037868  a(n)=(number of digits <=4)-(number of digits >4) in base 9
 *          representation of n.
 * https://oeis.org/A037868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, sign
 * Formula:   a(n) = (#digits <=4 in the base‑9 representation of n) -
 *            (#digits >4 in that representation)
 * Range:     i8 n <= 80, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* If n == 0 the value is 0, though the sequence is defined for n >= 1 */
    if (n == 0) return 0;
    idx_t t = n;
    seq_t low = 0;   /* count of digits 0,1,2,3,4 */
    seq_t high = 0;  /* count of digits 5,6,7,8 */
    while (t) {
        unsigned digit = t % 9;
        if (digit <= 4)
            low++;
        else
            high++;
        t /= 9;
    }
    return low - high;
}
