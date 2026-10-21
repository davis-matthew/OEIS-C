/*
 * A091959  a(1)=1, a(2n)=(a(n)+1) mod 2, a(2n+1)=2*a(2n)+1.
 * https://oeis.org/A091959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(2n) = (a(n)+1) mod 2; a(2n+1) = 2*a(2n)+1
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1u) return 1;
    if ((n & 1u) == 0u) {
        /* even: a(2k) = (a(k)+1) mod 2 */
        return (a(n >> 1) + 1) & 1u;
    } else {
        /* odd >1: a(2k+1) = 2*((a(k)+1) mod 2) + 1 */
        seq_t v = (a(n >> 1) + 1) & 1u;
        return (v << 1u) | 1u;
    }
}
