/*
 * A040471  Continued fraction for sqrt(494).
 * https://oeis.org/A040471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A040471: continued fraction for sqrt(494).
   a(0) = 22, and for n >= 1 the terms repeat with period 10:
   [4, 2, 2, 1, 2, 1, 2, 2, 4, 44] */

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)22;
    /* Periodic block of length 10 for n >= 1 */
    static const seq_t period[10] = {
        (seq_t)4, (seq_t)2, (seq_t)2, (seq_t)1,
        (seq_t)2, (seq_t)1, (seq_t)2, (seq_t)2,
        (seq_t)4, (seq_t)44
    };
    return period[(n - 1) % 10];
}
