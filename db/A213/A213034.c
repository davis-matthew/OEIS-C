/*
 * A213034  a(n) = floor(3*n/2)*floor(n/3).
 * https://oeis.org/A213034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(3*n/2)*floor(n/3) + (n%2 ? floor((n-1)/6) : 0)
 * Range:     u8 n <= 23, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute floor(3*n/2) */
    seq_t t1 = ((seq_t)n * 3) / 2;

    /* compute floor(n/3) */
    seq_t t2 = ((seq_t)n) / 3;

    seq_t prod = t1 * t2;

    /* add correction for odd n */
    if (n & 1ULL) {
        seq_t add = ((seq_t)(n - 1)) / 6;
        return prod + add;
    }
    return prod;
}
