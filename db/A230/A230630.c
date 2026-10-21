/*
 * A230630  a(1)=0; thereafter a(n) = (1 + a(floor(n/2))) mod 3.
 * https://oeis.org/A230630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(log2(n)) mod 3; equivalently, a(1)=0; for n>1, a(n)
 *            = (1 + a(floor(n/2))) mod 3
 * Range:     u8 n <= 9627, u16 n <= 9627, u32 n <= 9627, u64 n <= 9627,
 *            u128 n <= 9627, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle n >= 1; for n == 0 a(0) is undefined, return 0 as safe default. */
    if (n == 0) return 0;

    /* Compute floor(log2(n)) by counting shifts. */
    unsigned int lg = 0;
    idx_t v = n;
    while (v >> 1) {
        v >>= 1;
        ++lg;
    }
    return (seq_t)(lg % 3);
}
