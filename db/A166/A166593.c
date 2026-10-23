/*
 * A166593  Partial sums of A166592.
 * https://oeis.org/A166593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-3) - a(n-4) + a(n-5), initial
 *            values a(0)=0,a(1)=1,a(2)=4,a(3)=6,a(4)=9; the sequence has
 *            period 12
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence repeats every 12 terms.  The first 12 terms are
       known from the OEIS entry and satisfy the recurrence. */
    static const seq_t period[12] = {
        /* n =  0, 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11 */
        0,   1,   4,   6,   9,  10, 10,   9,   6,   4,  1,  0
    };
    return period[n % 12];
}
