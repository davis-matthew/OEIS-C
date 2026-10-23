/*
 * A173831  Largest prime < n^4.
 * https://oeis.org/A173831
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the largest prime strictly less than n^4
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 16, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function searches down from n^4 - 1 using the provided
 * is_prime_big() routine. It assumes that n ≥ 2 and that
 * the result fits in seq_t. */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Ensure n is at least 2; behaviour for n < 2 is undefined. */
    if (n < 2)
        return 0;

    seq_t nn = (seq_t)n;
    /* Compute n^4 using seq_t arithmetic. */
    seq_t limit = nn * nn * nn * nn;

    seq_t cand = limit - 1; /* strictly less than n^4 */
    while (!is_prime_big(cand))
        cand--;

    return cand;
}
