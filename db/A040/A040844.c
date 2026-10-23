/*
 * A040844  Continued fraction for sqrt(874).
 * https://oeis.org/A040844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 29, for n >= 1: a(n) = periodic[(n-1) mod 8] with
 *            periodic = [1, 1, 3, 2, 3, 1, 1, 58]
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns the n-th term of the continued fraction expansion of sqrt(874) */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)29;
    }
    /* Static array with the periodic part of the continued fraction */
    static const seq_t periodic[8] = {
        (seq_t)1, (seq_t)1, (seq_t)3, (seq_t)2,
        (seq_t)3, (seq_t)1, (seq_t)1, (seq_t)58
    };
    /* Compute index in the period */
    idx_t idx = (n - 1) % 8;
    return periodic[idx];
}
