/*
 * A195142  Concentric 10-gonal numbers.
 * https://oeis.org/A195142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n^2 + 3*(-1)^n - 3)/4
 * Range:     u8 n <= 10, u16 n <= 161, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equal to: a(2k)   = 10*k^2
 *            a(2k+1) = 10*k^2 + 10*k + 1
 */
seq_t a(idx_t n)
{
    if ((n & 1) == 0) {             /* even n: n = 2k */
        seq_t k = (seq_t)n >> 1;    /* k = n/2 */
        return (seq_t)10 * k * k;
    } else {                        /* odd n: n = 2k+1 */
        seq_t k = (seq_t)(n - 1) >> 1; /* k = (n-1)/2 */
        return (seq_t)10 * k * k + (seq_t)10 * k + 1;
    }
}
