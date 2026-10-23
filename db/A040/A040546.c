/*
 * A040546  Continued fraction for sqrt(570).
 * https://oeis.org/A040546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less, mult
 * Formula:   a(0)=23; for n>=1, a(n)=1,6,1,46 repeating with period 4
 *            starting at n=1
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)23;

    /* Periodic part of the continued fraction of sqrt(570) */
    static const seq_t period[] = {
        (seq_t)1,
        (seq_t)6,
        (seq_t)1,
        (seq_t)46
    };

    /* (n-1) & 3 gives the index 0..3 for the repeating period */
    return period[(n - 1) & 3];
}
