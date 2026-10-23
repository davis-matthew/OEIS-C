/*
 * A154127  Period 6: repeat [1, 2, 5, 8, 7, 4].
 * https://oeis.org/A154127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = a(n-1) - a(n-3) + a(n-4) for n > 3, with initial values
 *            a(0)=1, a(1)=2, a(2)=5, a(3)=8. This recurrence has period 6:
 *            [1, 2, 5, 8, 7, 4]
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t period[6] = {(seq_t)1, (seq_t)2, (seq_t)5,
                                (seq_t)8, (seq_t)7, (seq_t)4};

seq_t a(idx_t n)
{
    return period[n % 6];
}
