/*
 * A001540  Number of transpositions needed to generate permutations of length
 *          n.
 * https://oeis.org/A001540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*a(n-1) + n - (n mod 2), a(1) = 0
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    
    seq_t result = 0;
    for (idx_t i = 2; i <= n; i++) {
        result = (seq_t)i * result + (seq_t)i - ((seq_t)i & 1);
    }
    return result;
}
