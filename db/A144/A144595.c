/*
 * A144595  Christoffel word of slope 4/7.
 * https://oeis.org/A144595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = period[n mod 11] (period = 0,0,1,0,0,1,0,0,1,0,1)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[11] = {
        (seq_t)0, (seq_t)0, (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)1,
        (seq_t)0, (seq_t)0, (seq_t)1, (seq_t)0, (seq_t)1
    };
    return period[n % (idx_t)11];
}
