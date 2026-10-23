/*
 * A135537  Period 4: repeat [7, 5, 2, 4].
 * https://oeis.org/A135537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = period[ n mod 4 ] with period {7,5,2,4}. Equivalent
 *            recurrence: a(0)=7, a(1)=5, a(2)=2, a(3)=4, a(n)=a(n-4) for n>3
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    // Periodic pattern for the sequence.
    static const seq_t period[4] = { (seq_t)7, (seq_t)5, (seq_t)2, (seq_t)4 };

    // Index into the period using n mod 4.
    return period[n & 3];
}
