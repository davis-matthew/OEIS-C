/*
 * A046961  Bessel function Y_0(n) is a monotonically decreasing positive
 *          sequence.
 * https://oeis.org/A046961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=10, a(2)=26, a(3)=29, a(4)=70, a(5)=73, a(6)=117,
 *            a(7)=161, a(n)=183 + (n-8)*710 for n >= 8
 * Range:     u8 n <= 8, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base values for n = 0..7 */
    static const seq_t init[8] = {
        (seq_t)1,  (seq_t)10, (seq_t)26, (seq_t)29,
        (seq_t)70, (seq_t)73, (seq_t)117, (seq_t)161
    };
    if (n < 8)
        return init[n];
    seq_t sn = (seq_t)n;          /* cast index to seq_t */
    return (seq_t)183 + (sn - (seq_t)8) * (seq_t)710;
}
