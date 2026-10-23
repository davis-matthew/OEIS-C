/*
 * A144478  Period 9: repeat [1, 0, 5, 7, 6, 2, 4, 3, 8].
 * https://oeis.org/A144478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = [1,0,5,7,6,2,4,3,8] periodic with period 9, a(1)=1
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[9] = {
        (seq_t)1, (seq_t)0, (seq_t)5, (seq_t)7, (seq_t)6,
        (seq_t)2, (seq_t)4, (seq_t)3, (seq_t)8
    };
    idx_t idx = (n - 1) % 9;
    return period[idx];
}
