/*
 * A232545  Number of Euler tours of the complete digraph on n vertices.
 * https://oeis.org/A232545
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = n^(n-2)*((n-2)!)^n
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"   /* provides seq_t big_pow(seq_t, seq_t) */
#include "../utils/factorial.h"/* provides seq_t factorial(idx_t) */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;  /* undefined for n < 2 */

    seq_t pow_part   = big_pow((seq_t)n, (seq_t)(n - 2));   /* n^(n-2)     */
    seq_t fact       = factorial(n - 2);                     /* (n-2)!      */
    seq_t fact_pow   = big_pow(fact, (seq_t)n);              /* (n-2)!^n    */
    return pow_part * fact_pow;                             /* result      */
}
