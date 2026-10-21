/*
 * A233931  a(2n) = a(n) + n, a(2n+1) = a(n), with a(0)=0.
 * https://oeis.org/A233931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0}^{⌊log₂ n⌋} (1 - bittest(n,k)) * ⌊n / 2^{k+1}⌋
 *            (i.e. for every zero bit in the binary representation of n, add
 *            the integer formed by the bits left of that zero. Recurrence:
 *            a(2n) = a(n) + n, a(2n+1) = a(n), a(0) = 0.)
 * Range:     u8 n <= 271, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 0; (n >> i); ++i) {
        if (((n >> i) & 1) == 0) {
            sum += (seq_t)(n >> (i + 1));
        }
    }
    return sum;
}
