/*
 * A080049  Operation count to create all permutations of n distinct elements
 *          using Algorithm L (lexicographic permutation generation) from
 *          Knuth's The Art of Computer Programming, Vol. 4, chapter 7.2.1.2.
 *          Sequence gives number of interchange operations in step L4.
 * https://oeis.org/A080049
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2) = 0 a(n) = n * a(n-1) + (n-1) * floor((n-1)/2) for n >= 3
 *            (Implementing the lexicographic permutation operation count.)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* No defined behaviour for n < 2, but we return 0 for safety. */
    if (n <= 2)
        return (seq_t)0;

    seq_t a_prev = (seq_t)0; /* a(2) */
    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t term1 = (seq_t)i * a_prev;                 /* n * a(n-1)      */
        seq_t term2 = (seq_t)(i - 1) * ((i - 1) / 2);    /* (n-1)*floor((n-1)/2) */
        a_prev = term1 + term2;
    }
    return a_prev;
}
