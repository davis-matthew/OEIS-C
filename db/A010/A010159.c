/*
 * A010159  Continued fraction for sqrt(86).
 * https://oeis.org/A010159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   Continued fraction terms of sqrt(86). The sequence has period 10
 *            after the initial term
 * Range:     u8 n <= 9911, u16 n <= 9911, u32 n <= 9911, u64 n <= 9911,
 *            u128 n <= 9911, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic part after a(0) */
    static const seq_t pat[10] = { (seq_t)3, (seq_t)1, (seq_t)1, (seq_t)1,
                                   (seq_t)8, (seq_t)1, (seq_t)1, (seq_t)1,
                                   (seq_t)3, (seq_t)18 };
    if (n == 0u)
        return (seq_t)9;
    /* a(n) for n >= 1 */
    return pat[(n - 1u) % (idx_t)10];
}
