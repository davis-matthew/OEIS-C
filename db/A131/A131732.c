/*
 * A131732  a(4*k) = 2*k+1, a(4*k+1) = -4*k-3, a(4*k+2) = 2*k+2, a(4*k+3) = 0.
 * https://oeis.org/A131732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(4*k) = 2*k+1, a(4*k+1) = -4*k-3, a(4*k+2) = 2*k+2, a(4*k+3) =
 *            0
 * Range:     i8 n <= 128, i16 n <= 9999, i32 n <= 9999, i64 n <= 9999,
 *            i128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n >> 2;      /* k = floor(n/4) */
    switch (n & 3) {       /* n mod 4 */
        case 0: return (seq_t)(k * 2 + 1);
        case 1: return -(seq_t)(4 * k + 3);
        case 2: return (seq_t)(2 * k + 2);
        case 3: return (seq_t)0;
    }
    /* unreachable, but silence compiler warnings */
    return (seq_t)0;
}
