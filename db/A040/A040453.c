/*
 * A040453  Continued fraction for sqrt(475).
 * https://oeis.org/A040453
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

/* Continued fraction terms of sqrt(475).  a(0)=21; for n≥1 the sequence
   repeats the period [1,3,1,6,2,6,1,3,1,42] with length 10. */
seq_t a(idx_t n)
{
    static const seq_t period[10] = {
        (seq_t)1, (seq_t)3, (seq_t)1, (seq_t)6, (seq_t)2,
        (seq_t)6, (seq_t)1, (seq_t)3, (seq_t)1, (seq_t)42
    };
    if (n == 0)
        return (seq_t)21;
    return period[(n - 1) % 10];
}
