/*
 * A041830  Numerators of continued fraction convergents to sqrt(436).
 * https://oeis.org/A041830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(0)=20, a(1)=21, * a(n)=c_{n-1}*a(n-1)+a(n-2) (n>=2) * where
 *            c_k are the partial quotients of sqrt(436): * c =
 *            1,7,2,1,1,1,13,3,2,2,5,1,1,4,10,4, *
 *            1,1,5,2,2,3,13,1,1,1,2,7,1,40 (period 30)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)20;
    if (n == 1) return (seq_t)21;

    static const unsigned char cf[30] = {
        1,7,2,1,1,1,13,3,2,2,5,1,1,4,10,4,
        1,1,5,2,2,3,13,1,1,1,2,7,1,40
    };

    seq_t a0 = (seq_t)20;
    seq_t a1 = (seq_t)21;
    for (idx_t i = 2; i <= n; ++i) {
        unsigned char coeff = cf[(i - 1) % 30];
        /* Multiply coefficient and previous value then add the one before it */
        seq_t next = (seq_t)coeff * a1 + a0;
        a0 = a1;
        a1 = next;
    }
    return a1;
}
