/*
 * A111654  n appears n-th composite number times.
 * https://oeis.org/A111654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, less, nonn
 * Formula:   a(n) = the integer k such that ∑_{i=1}^{k-1} A002808(i) < n ≤
 *            ∑_{i=1}^{k} A002808(i), where A002808(i) is the i‑th composite
 *            number
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, but safe */

    /* cumulative length of the sequence */
    seq_t cum = 0;

    /* run length index (k) */
    idx_t k = 0;

    /* current integer to test for compositeness */
    idx_t m = 4;

    while (cum < (seq_t)n)
    {
        /* find next composite number */
        while (is_prime(m)) m++;

        /* this is the run length A002808(k+1) = m */
        k++;

        /* add to cumulative sum (use seq_t arithmetic) */
        cum += (seq_t)m;

        /* move to next candidate */
        m++;
    }

    return (seq_t)k;
}
