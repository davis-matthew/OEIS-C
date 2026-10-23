/*
 * A018931  The number of permutations of n cards in which 2 is the first card
 *          hit and 3 the next hit after 2.
 * https://oeis.org/A018931
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-4)*(n-3)! for n > 4; a(3)=0, a(4)=1
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 14, u64 n <= 22, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;
    if (n == 3) return 0;
    if (n == 4) return 1;

    seq_t fact = 1;
    for (idx_t i = 2; i <= n - 3; ++i)
        fact *= (seq_t)i;          /* factorial (n-3)! */

    return ((seq_t)(n - 4)) * fact; /* (n-4)*(n-3)! */
}
