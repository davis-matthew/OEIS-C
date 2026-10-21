/*
 * A093052  Exponent of 2 in 6^n - 2^n.
 * https://oeis.org/A093052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0; for n>0, a(n)=n+1 if n is odd, a(n)=n+2+v2(n) if n is
 *            even, where v2(n) is the exponent of 2 in n
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return 0;

    seq_t result = (seq_t)n;
    if ((n & 1u) == 1u) {
        /* n is odd */
        result += 1;
    } else {
        /* n is even: compute v2(n) */
        idx_t tmp = n;
        unsigned int tz = 0;
        while ((tmp & 1u) == 0u) {
            tz++;
            tmp >>= 1;
        }
        result += 2;            /* +2 part of the formula */
        result += (seq_t)tz;    /* +v2(n) part */
    }
    return result;
}
