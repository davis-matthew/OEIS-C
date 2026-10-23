/*
 * A121663  a(0) = 1; if n = 2^k, a(n) = k+2, otherwise
 *          a(n)=(A000523(n)+2)*a(A053645(n)).
 * https://oeis.org/A121663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{k set in binary n} (k+2), a(0)=1
 * Range:     u8 n <= 29, u16 n <= 246, u32 n <= 4094, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t x = n;
    while (x) {
        /* position of the least significant set bit */
        idx_t t = 0;
        while (((x >> t) & 1) == 0) t++;
        result *= (seq_t)(t + 2);
        x &= x - 1; /* clear the lowest set bit */
    }
    return result;
}
