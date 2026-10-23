/*
 * A040299  Continued fraction for sqrt(317).
 * https://oeis.org/A040299
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

/* Continued fraction of √317
   a(0) = 17
   for n > 0: a(n) follows a period of length 11:
   {1,4,8,1,2,2,1,8,4,1,34} repeated. */
seq_t a(idx_t n)
{
    /* period of length 11 for n > 0 */
    static const seq_t period[11] = {
        (seq_t)1, (seq_t)4, (seq_t)8, (seq_t)1, (seq_t)2,
        (seq_t)2, (seq_t)1, (seq_t)8, (seq_t)4, (seq_t)1,
        (seq_t)34
    };
    if (n == 0)
        return (seq_t)17;
    /* modulus is small; idx_t is unsigned, no overflow problems */
    return period[(n - 1) % 11];
}
