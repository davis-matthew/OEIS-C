/*
 * A117445  Periodic {0,-1,1,4,-1,4,-4,-4,1,1,-4,-4,4,-1,4,1,-1} (period 17).
 * https://oeis.org/A117445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   Period 0,-1,1,4,-1,4,-4,-4,1,1,-4,-4,4,-1,4,1,-1 (period 17)
 * Range:     i8 n <= 79, i16 n <= 79, i32 n <= 79, i64 n <= 79, i128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic array of the 17 terms */
    static const seq_t period[17] = {
        0, -1, 1, 4, -1, 4, -4, -4, 1, 1, -4, -4, 4, -1, 4, 1, -1
    };
    return period[n % 17];
}
