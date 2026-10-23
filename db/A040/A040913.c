/*
 * A040913  Continued fraction for sqrt(944).
 * https://oeis.org/A040913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by continued fraction expansion of sqrt(944) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)30;

    /* Periodic part of the continued fraction for sqrt(944) */
    static const seq_t period[16] = {
        (seq_t)1, (seq_t)2, (seq_t)1, (seq_t)1,
        (seq_t)1, (seq_t)2, (seq_t)2, (seq_t)3,
        (seq_t)2, (seq_t)2, (seq_t)1, (seq_t)1,
        (seq_t)1, (seq_t)2, (seq_t)1, (seq_t)60
    };

    idx_t idx = (n - 1) % 16;
    return period[idx];
}
