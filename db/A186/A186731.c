/*
 * A186731  a(3n) = 2n, a(3n+1) = n, a(3n+2) = n+1.
 * https://oeis.org/A186731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(3n) = 2n, a(3n+1) = n, a(3n+2) = n+1
 * Range:     u8 n <= 383, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n / 3;
    switch (n % 3) {
        case 0:
            return (seq_t)k * 2;
        case 1:
            return (seq_t)k;
        case 2:
            return (seq_t)k + 1;
    }
    /* every n yields one of the above cases */
    return 0;
}
