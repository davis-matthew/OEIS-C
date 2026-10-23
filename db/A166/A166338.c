/*
 * A166338  a(n) = (4*n)!/n!.
 * https://oeis.org/A166338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n)!/n! (OEIS A166338)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Computes a(n) = (4*n)! / n! as a product (n+1)(n+2)...(4n)   */
seq_t a(idx_t n)
{
    /* Handle n = 0 explicitly; the product loop below also yields 1,
       but avoiding the loop keeps the function trivially correct.   */
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t result = (seq_t)1;
    seq_t start   = (seq_t)n;         /* start = n (will be incremented to n+1) */
    /* Number of terms in the product: from n+1 to 4n inclusive.   */
    idx_t terms = 3 * n;

    for (idx_t k = 1; k <= terms; ++k) {
        result *= (start + (seq_t)k);
    }
    return result;
}
