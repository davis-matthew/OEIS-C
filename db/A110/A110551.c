/*
 * A110551  Period 6: repeat [1, 3, 5, 5, 3, 1].
 * https://oeis.org/A110551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1, 3, 5, 5, 3, 1 repeating with period 6
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic values for n mod 6 */
    static const seq_t period[6] = {
        (seq_t)1, /* n mod 6 == 0 */
        (seq_t)3, /* n mod 6 == 1 */
        (seq_t)5, /* n mod 6 == 2 */
        (seq_t)5, /* n mod 6 == 3 */
        (seq_t)3, /* n mod 6 == 4 */
        (seq_t)1  /* n mod 6 == 5 */
    };
    return period[n % 6];
}
