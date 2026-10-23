/*
 * A040708  Continued fraction for sqrt(736).
 * https://oeis.org/A040708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   continued fraction terms of sqrt(736)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 27, thereafter periodic part: [7,1,2,1,2,1,7,54] repeating. */
seq_t a(idx_t n)
{
    /* Periodic sequence after the initial term */
    static const seq_t period[8] = { (seq_t)7, (seq_t)1, (seq_t)2,
                                     (seq_t)1, (seq_t)2, (seq_t)1,
                                     (seq_t)7, (seq_t)54 };
    if (n == 0) {
        return (seq_t)27;
    }
    idx_t idx = (n - 1) % 8;
    return period[idx];
}
