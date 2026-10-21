/*
 * A079885  Number of index tests required to create all permutations of n
 *          distinct elements using the "streamlined" version of Algorithm L
 *          (lexicographic permutation generation) from Knuth's The Art of
 *          Computer Programming, Vol. 4, chapter 7.2.1.2.
 * https://oeis.org/A079885
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(3) = 0; for n >= 4, a(n) = n*a(n-1) + 1 + (n-1)*floor((n-1)/2)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset: a(3) = 0; values below are irrelevant */
    if (n <= 3)
        return 0;

    seq_t val = 0;          /* a(3) */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t ii    = (seq_t)i;
        seq_t term  = ii * val;                            /* n * a(n-1)  */
        seq_t add   = 1 + (seq_t)(i - 1) * ((seq_t)(i - 1) / 2); /* 1 + (n-1)*floor((n-1)/2) */
        val = term + add;
    }
    return val;
}
