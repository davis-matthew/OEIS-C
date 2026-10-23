/*
 * A110547  Number of sides of regular polygons whose interior angles (in
 *          degrees) are not integers.
 * https://oeis.org/A110547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n-th positive integer that is not a divisor of 360
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 7, a(2) = 11, a(3) = 13, ...  (sequence A110547) */

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t i = 1;
    while (1) {
        /* 360 % i == 0  <=>  i divides 360 */
        if ((360 % i) != 0) {
            ++count;
            if (count == n) {
                return (seq_t)i;
            }
        }
        ++i;
    }
}
