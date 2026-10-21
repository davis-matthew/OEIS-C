/*
 * A081496  Start with Pascal's triangle; a(n) is the sum of the numbers on
 *          the periphery of the n-th central rhombus containing exactly 4
 *          numbers.
 * https://oeis.org/A081496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n-4)*Catalan(n-1) = (9*n-4)*binomial(2*(n-1),(n-1))/n
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                     /* undefined for n<1, just return 0  */

    /* n-1 is the index of the Catalan number */
    idx_t k    = n - 1;
    seq_t bin  = binomial(2 * k, k);          /* binomial(2k, k)  */
    seq_t cat  = bin / (seq_t)n;               /* Catalan(n-1)     */
    seq_t fact = ((seq_t)9 * (seq_t)n - (seq_t)4);

    return fact * cat;
}
