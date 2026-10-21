/*
 * A204890  Ordered differences of primes.
 * https://oeis.org/A204890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: For n≥1, order all ordered pairs (i,j) with 1≤i<j by increasing j,
   then increasing i. The nth term a(n) is the prime difference
   p_j − p_i, where j is the smallest integer with j(j−1)/2 ≥ n
   and i = n − (j−2)(j−1)/2. */
#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    idx_t j = 2;
    idx_t cum = 1;                       /* (j-1)*j/2 for j=2 */
    while (cum < n) {
        ++j;
        cum += j - 1;                    /* add new rows of differences */
    }

    /* cumulative count up to row j, and previous row count */
    idx_t prev = cum - (j - 1);          /* (j-2)*(j-1)/2 */
    idx_t i = n - prev;                  /* 1‑based index in the primes list */

    return (seq_t)get_prime(j) - (seq_t)get_prime(i);
}
