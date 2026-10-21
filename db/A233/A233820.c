/*
 * A233820  Period 4: repeat [20, 5, 15, 10].
 * https://oeis.org/A233820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [20,5,15,10] repeating; a(1)=20, a(2)=5, a(3)=15,
 *            a(4)=10; a(n)=a(n-4) for n>4
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[4] = {20, 5, 15, 10};
    if (n == 0) return 0;          /* defined only for n≥1, guard for safety */
    return period[(n - 1) % 4];
}
