/*
 * A040612  Continued fraction for sqrt(638).
 * https://oeis.org/A040612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* CF sqrt(638): a(0)=25; for n≥1 periodic of length 8: {3,1,6,2,6,1,3,50} */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)25;
    static const seq_t period[8] = { (seq_t)3, (seq_t)1, (seq_t)6,
                                     (seq_t)2, (seq_t)6, (seq_t)1,
                                     (seq_t)3, (seq_t)50 };
    return period[(n - 1) % 8];
}
