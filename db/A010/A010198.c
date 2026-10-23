/*
 * A010198  Continued fraction for sqrt(139).
 * https://oeis.org/A010198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic continued fraction for sqrt(139) */
seq_t a(idx_t n)
{
    /* a(0) = 11, thereafter period of length 18: */
    /* 1, 3, 1, 3, 7, 1, 1, 2, 11, 2, 1, 1, 7, 3, 1, 3, 1, 22 */
    if (n == 0)
        return (seq_t)11;

    static const seq_t period[18] = {
        (seq_t)1, (seq_t)3, (seq_t)1, (seq_t)3, (seq_t)7,
        (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)11, (seq_t)2,
        (seq_t)1, (seq_t)1, (seq_t)7, (seq_t)3, (seq_t)1,
        (seq_t)3, (seq_t)1, (seq_t)22
    };

    /* n >= 1, so (n-1) fits into uint64_t and %18 is safe */
    return period[(n - 1) % 18];
}
