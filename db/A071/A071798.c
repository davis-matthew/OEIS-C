/*
 * A071798  Number of paths on the surface of the n-dimensional lattice
 *          [0..2]^n; i.e., the lattice paths that do not pass through the
 *          point (1,1,...,1).
 * https://oeis.org/A071798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = (2n)!/2^n - (n!)^2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent combinatorial form: a(n) = (n! * (2n-1)!!) - (n!)^2   */
seq_t a(idx_t n)
{
    /* Offset is 1. For n == 0 return 0 as a fallback. */
    if (n == (idx_t)0)
        return (seq_t)0;

    seq_t fact_n = (seq_t)1;    /* n!   */
    seq_t odd_prod = (seq_t)1;  /* (2n-1)!! = product of odd numbers up to 2n-1 */

    for (idx_t i = (idx_t)1; i <= n; ++i) {
        fact_n    = fact_n * (seq_t)i;
        odd_prod = odd_prod * (seq_t)((idx_t)2 * i - (idx_t)1);
    }

    seq_t term1 = fact_n * odd_prod; /* (2n)!/2^n  */
    seq_t term2 = fact_n * fact_n;   /* (n!)^2    */

    return term1 - term2;
}
