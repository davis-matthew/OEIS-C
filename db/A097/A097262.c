/*
 * A097262  Numbers whose set of base 16 digits is {0,F}, where F base 16 = 15
 *          base 10.
 * https://oeis.org/A097262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 15*A033052(n); Recurrence: a(0)=0, a(2n)=16*a(n),
 *            a(2n+1)=a(2n)+15
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function interprets the binary representation of n as a
   sequence of hex digits 0 or F.  For each bit, starting from the
   most significant, we shift the accumulated value left by one
   hex digit (×16) and, if the bit is 1, add 15 (the hex digit F). */

seq_t a(idx_t n) {
    seq_t res = 0;
    if (n == 0) return 0;

    /* Find the position of the most significant set bit. */
    idx_t m = n;
    idx_t msb = 0;
    while (m >>= 1) msb++;

    /* Process bits from most significant to least significant. */
    for (idx_t i = msb; ; --i) {
        res <<= 4;                     /* shift left by one hex digit */
        if ((n >> i) & 1) res += 15;   /* add F if the bit is 1 */
        if (i == 0) break;             /* finished all bits */
    }
    return res;
}
