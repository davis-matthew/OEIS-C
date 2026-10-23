/*
 * A164682  a(n) = 2*a(n-2) for n > 2; a(1) = 5, a(2) = 8.
 * https://oeis.org/A164682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) for n>2; a(1)=5, a(2)=8
 * Range:     u8 n <= 11, u16 n <= 27, u32 n <= 59, u64 n <= 123,
 *            u128 n <= 251, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 5;
    if (n == 2) return 8;
    seq_t val;
    if (n & 1) {           /* odd index */
        val = 5;
        idx_t shift = (n - 1) / 2;
        while (shift--) val = val * 2;
    } else {                /* even index */
        val = 8;
        idx_t shift = (n - 2) / 2;
        while (shift--) val = val * 2;
    }
    return val;
}
