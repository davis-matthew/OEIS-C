/*
 * A126951  List of pairs: k followed by k^3.
 * https://oeis.org/A126951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)/2 if n is odd, a(n) = (n/2)^3 otherwise
 * Range:     u8 n <= 13, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Odd index: return the integer sequence (n+1)/2
       Even index: return (n/2)^3   */
    if ((n & 1ULL) == 0ULL) {
        seq_t k = (seq_t)(n >> 1);
        return k * k * k;
    } else {
        return ((seq_t)(n >> 1)) + 1;
    }
}
