/*
 * A058333  Number of 3 X 3 matrices with elements from [0,...,(n-1)]
 *          satisfying the condition that the middle element of each row or
 *          column is the difference of the two end elements (in absolute
 *          value).
 * https://oeis.org/A058333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n⁴ + 8·n³ - 10·n² + 4·n) / 3 (OEIS A058333)
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 259, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>    /* for uint64_t if needed by other headers */

seq_t a(idx_t n)
{
    if (n == 0)          /* value is undefined for offset 1, return 0 */
        return (seq_t)0;

    /* convert index to seq_t once to keep all arithmetic in seq_t */
    seq_t ns = (seq_t)n;

    /* compute n², n³ and n⁴ */
    seq_t n2 = ns * ns;               /* n² */
    seq_t n3 = n2 * ns;               /* n³ */
    seq_t n4 = n2 * n2;               /* n⁴ */

    /* accumulate the polynomial terms.
       The combination n⁴ + 8·n³ + 4·n is always >= 10·n² for n ≥ 1,
       so the subtraction cannot underflow unsigned arithmetic. */
    seq_t term = n4
               + (seq_t)8 * n3
               + (seq_t)4 * ns;

    term -= (seq_t)10 * n2;           /* subtract 10·n² */

    /* division by 3; the result is integer for all n ≥ 1 */
    return term / (seq_t)3;
}
