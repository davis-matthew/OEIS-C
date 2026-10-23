/*
 * A032828  Numbers whose set of base-16 digits is {1,4}.
 * https://oeis.org/A032828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=1, a(2)=4; for n≥3, a(n)=16*a(floor(n/2))+1 if n is odd,
 *            a(n)=16*a(floor((n-1)/2))+4 if n is even
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 510, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2) {
        return (n == 1) ? (seq_t)1 : (seq_t)4;
    }
    if (n & 1) {                /* odd n */
        return (a(n >> 1) << 4) + (seq_t)1;
    }
    /* even n */
    return (a((n - 1) >> 1) << 4) + (seq_t)4;
}
