/*
 * A061462  The exact power of 2 that divides the n-th Bell number (A000110).
 *          Has period 12.
 * https://oeis.org/A061462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = table[n % 12]
 * The table contains the exact power of 2 dividing the n-th Bell number
 * for a period of 12.  The sequence is periodic: 1,1,2,1,1,4,1,1,4,
 * 1,1,2.
 */
seq_t a(idx_t n)
{
    static const seq_t period[12] = {
        1, 1, 2, 1, 1, 4,
        1, 1, 4, 1, 1, 2
    };
    return period[n % 12];
}
