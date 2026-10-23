/*
 * A154028  a(2n) = n*(n+1)/2, a(2n+1) = n!.
 * https://oeis.org/A154028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)/2 if n is even, a(n) = ((n-1)/2)! if n is odd
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 26, u64 n <= 42, u128 n <= 70,
 *            big any n
 * Helpers:   fact_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* offset 0 */

#include "../utils/fact_upto.h"

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {                      /* even index */
        idx_t k = n >> 1;                    /* k = n/2 */
        return (((seq_t)k * ((seq_t)k + 1)) / 2);  /* triangular number */
    } else {                                 /* odd index */
        idx_t k = (n - 1) >> 1;              /* k = (n-1)/2 */
        return fact_upto(k);                 /* factorial */
    }
}
