/*
 * A070357  a(n) = 3^n mod 28.
 * https://oeis.org/A070357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n mod 28
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodicity: 3^n mod 28 repeats every 6 */
seq_t a(idx_t n)
{
    /* Small table containing one full period */
    static const seq_t period[6] = {
        (seq_t)1,
        (seq_t)3,
        (seq_t)9,
        (seq_t)27,
        (seq_t)25,
        (seq_t)19
    };
    /* Compute the index within the period */
    idx_t r = n % 6;
    return period[r];
}
