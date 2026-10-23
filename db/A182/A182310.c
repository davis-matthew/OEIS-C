/*
 * A182310  a(0)=0, a(n+1) = (a(n) XOR floor(a(n)/2)) + 1, where XOR is the
 *          bitwise exclusive-or operator.
 * https://oeis.org/A182310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0; a(n+1) = (a(n) XOR floor(a(n)/2)) + 1
 * Range:     u8 n <= 104, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 0;
    /* Iterate from 0 up to n-1, applying the recurrence */
    for (idx_t i = 0; i < n; ++i) {
        /* floor(a/2) is just the right shift of the unsigned integer */
        seq_t half = val >> 1;
        /* Next term: (a XOR floor(a/2)) + 1 */
        val = (val ^ half) + 1;
    }
    return val;
}
