/*
 * A195031  Multiples of 5 and of 12 interleaved: a(2n-1) = 5n, a(2n) = 12n.
 * https://oeis.org/A195031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n-1) = 5n, a(2n) = 12n, offset = 1
 * Range:     u8 n <= 43, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0; /* undefined for offsets < 1 */
    }
    if (n & 1U) {                       /* odd index → 5n */
        seq_t k = ((seq_t)n + 1) / 2;    /* k = (n+1)/2 */
        return (seq_t)5 * k;
    } else {                            /* even index → 12n */
        seq_t k = (seq_t)n / 2;          /* k = n/2 */
        return (seq_t)12 * k;
    }
}
