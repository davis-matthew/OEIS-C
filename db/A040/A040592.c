/*
 * A040592  Continued fraction for sqrt(617).
 * https://oeis.org/A040592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(0)=24; for n>0, a(n) = period[(n-1) mod 15], where period =
 *            {1,5,4,2,1,6,2,2,6,1,2,4,5,1,48}
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return the integer part of sqrt(617) */
    if (n == 0) return (seq_t)24;

    static const seq_t period[15] = {
        (seq_t)1,  (seq_t)5,  (seq_t)4,  (seq_t)2,  (seq_t)1,
        (seq_t)6,  (seq_t)2,  (seq_t)2,  (seq_t)6,  (seq_t)1,
        (seq_t)2,  (seq_t)4,  (seq_t)5,  (seq_t)1,  (seq_t)48
    };

    /* Compute position in the repeating block */
    idx_t pos = (n - 1) % 15;
    return period[pos];
}
