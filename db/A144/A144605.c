/*
 * A144605  Christoffel word of slope 7/11.
 * https://oeis.org/A144605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-18). Periodic sequence of length 18
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[18] = {
        0,0,1,0,0,1,0,1,0,0,1,0,1,0,0,1,0,1
    };
    idx_t idx = n % 18;
    return period[idx];
}
