/*
 * A010134  Continued fraction for sqrt(43).
 * https://oeis.org/A010134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, cofr
 * Formula:   a(0) = 6, a(n) = a(n-10) for n > 10, period =
 *            {1,1,3,1,5,1,3,1,1,12}
 * Range:     u8 n <= 9547, u16 n <= 9547, u32 n <= 9547, u64 n <= 9547,
 *            u128 n <= 9547, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)6;
    }
    static const seq_t period[10] = {
        (seq_t)1,
        (seq_t)1,
        (seq_t)3,
        (seq_t)1,
        (seq_t)5,
        (seq_t)1,
        (seq_t)3,
        (seq_t)1,
        (seq_t)1,
        (seq_t)12
    };
    idx_t mod = (n - (idx_t)1) % (idx_t)10;
    return period[mod];
}
