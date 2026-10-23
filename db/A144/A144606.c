/*
 * A144606  Christoffel word of slope 8/11.
 * https://oeis.org/A144606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-19). Period: 19. The first 19 terms are
 *            a(0)=0,a(1)=0,a(2)=1,a(3)=0,a(4)=1,a(5)=0,a(6)=0,a(7)=1,
 *            a(8)=0,a(9)=1,a(10)=0,a(11)=1,a(12)=0,a(13)=0,a(14)=1,
 *            a(15)=0,a(16)=1,a(17)=0,a(18)=1. All further terms repeat this
 *            pattern
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic table for one full cycle (19 values). */
    static const seq_t period[19] = {
        (seq_t)0, (seq_t)0, (seq_t)1, (seq_t)0, (seq_t)1,
        (seq_t)0, (seq_t)0, (seq_t)1, (seq_t)0, (seq_t)1,
        (seq_t)0, (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)1,
        (seq_t)0, (seq_t)1, (seq_t)0, (seq_t)1
    };
    /* Since a(n) = a(n-19) for all n, we only need n mod 19. */
    idx_t idx = n % (idx_t)19;
    return period[idx];
}
