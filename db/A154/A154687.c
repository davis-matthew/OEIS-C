/*
 * A154687  Period 6: repeat [1, 4, 7, 8, 5, 2].
 * https://oeis.org/A154687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) repeats the pattern [1, 4, 7, 8, 5, 2] with period 6
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t cycle[6] = {
        (seq_t)1, (seq_t)4, (seq_t)7,
        (seq_t)8, (seq_t)5, (seq_t)2
    };
    return cycle[n % 6];
}
