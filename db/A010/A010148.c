/*
 * A010148  Continued fraction for sqrt(69).
 * https://oeis.org/A010148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   a(0)=8; a(n)=period[(n-1)%8] where period={3,3,1,4,1,3,3,16}
 * Range:     u8 n <= 9864, u16 n <= 9864, u32 n <= 9864, u64 n <= 9864,
 *            u128 n <= 9864, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The continued fraction of sqrt(69) has period 8 after the initial term 8. */
    if (n == 0)
        return (seq_t)8;

    static const seq_t period[8] = {
        (seq_t)3, (seq_t)3, (seq_t)1, (seq_t)4,
        (seq_t)1, (seq_t)3, (seq_t)3, (seq_t)16
    };

    /* (n-1) fits in idx_t; modulo 8 selects the correct repeat. */
    return period[(n - 1) % 8];
}
