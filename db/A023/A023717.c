/*
 * A023717  Numbers with no 3's in base-4 expansion.
 * https://oeis.org/A023717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = ∑_{i≥0} d(i)*4^i where d(i) are the base‑3 digits of n
 * Range:     u8 n <= 80, u16 n <= 6560, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t pow4   = 1;
    idx_t nn = n;
    while (nn > 0) {
        unsigned digit = (unsigned)(nn % 3);
        result += (seq_t)digit * pow4;
        pow4 *= (seq_t)4;
        nn /= 3;
    }
    return result;
}
