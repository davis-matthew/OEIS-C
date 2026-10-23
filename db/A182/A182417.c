/*
 * A182417  a(0)=0, a(n+1) = (a(n) XOR floor(a(n)/4)) + 1, where XOR is the
 *          bitwise exclusive-or operator.
 * https://oeis.org/A182417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(0) = 0; a(n+1) = (a(n) XOR floor(a(n)/4)) + 1
 * Range:     u8 n <= 135, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)0;
    for (idx_t i = 0; i < n; ++i) {
        seq_t q = val >> 2;          /* floor(a(n)/4) */
        val = (val ^ q) + (seq_t)1;  /* a(n+1) */
    }
    return val;
}
