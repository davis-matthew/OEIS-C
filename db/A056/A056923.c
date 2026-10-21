/*
 * A056923  Write the integers in groups: 0; 1,2; 3,4,5; 6,7,8,9; ... and form
 *          the product of the members of each group.
 * https://oeis.org/A056923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=0}^{n} (n(n+1)/2 + k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The product for group n runs over n+1 consecutive integers.
       The starting integer is the nth triangular number T(n) = n(n+1)/2,
       and the ending integer is T(n)+n = n(n+3)/2. */
    seq_t start = (seq_t)n * ((seq_t)n + 1) / 2;
    seq_t end   = (seq_t)n * ((seq_t)n + 3) / 2;

    seq_t prod = 1;
    for (seq_t i = start; i <= end; ++i)
        prod *= i;

    return prod;
}
