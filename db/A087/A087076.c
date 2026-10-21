/*
 * A087076  Sums of the squares of the elements in the subsets of the integers
 *          1 to n.
 * https://oeis.org/A087076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-2)*n*(n+1)*(2*n+1)/3 (OEIS A087076)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 21, u64 n <= 49, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the small indices directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Compute the polynomial part n*(n+1)*(2*n+1)/3 */
    seq_t nn = (seq_t)n;
    seq_t t = nn * (nn + 1) * ((seq_t)2 * nn + 1);
    t /= (seq_t)3;          /* exact division; n*(n+1)*(2*n+1) is always divisible by 3 */

    /* Multiply by 2^(n-2) by repeated doubling (no left‑shift on narrow types) */
    seq_t res = t;
    for (idx_t i = 0; i + 2 < n; ++i) {
        res = res + res;    /* double the value */
    }
    return res;
}
