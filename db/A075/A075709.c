/*
 * A075709  Upper irredundance number for kings graph K_n on n^2 nodes.
 * https://oeis.org/A075709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = { ceil(n/2)^2 for odd n, (n/2)^2 + max(0, n/2 - 3) for
 *            even n }
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the OEIS sequence A075709: upper irredundance number
   for the king's graph on an n×n board. */
seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 in OEIS indexing, but handle n=0 safely */
    if (n == 0)
        return 0;

    /* odd n: use m = ceil(n/2) */
    if (n & 1) {
        seq_t m = (seq_t)n / 2 + 1;   /* ceil division by 2 */
        return m * m;
    }

    /* even n: k = n/2 */
    seq_t k = (seq_t)n / 2;
    seq_t offset = k > 3 ? (k - 3) : 0;
    return k * k + offset;
}
