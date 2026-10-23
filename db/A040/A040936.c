/*
 * A040936  Continued fraction for sqrt(968).
 * https://oeis.org/A040936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A040936: Continued fraction for sqrt(968) */
/*  a(0) = 31, thereafter the period is [8,1,6,1,8,62] with length 6 */
seq_t a(idx_t n)
{
    static const seq_t period[6] = {
        (seq_t)8,
        (seq_t)1,
        (seq_t)6,
        (seq_t)1,
        (seq_t)8,
        (seq_t)62
    };
    if (n == 0)
        return (seq_t)31;
    return period[(n - 1) % 6];
}
