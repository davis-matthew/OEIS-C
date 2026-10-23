/*
 * A027863  Numbers k such that k^2 + (k+1)^2 + (k+2)^2 is prime.
 * https://oeis.org/A027863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 18, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th integer k (k ≥ 0) such that k² + (k+1)² + (k+2)² is prime. */
/* The sequence is ordered increasingly by k. */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    idx_t count = 0;    /* number of terms found so far */
    idx_t k = 0;        /* candidate integer k */

    while (1) {
        seq_t ks = (seq_t)k;
        seq_t sum = ks*ks + (ks+1)*(ks+1) + (ks+2)*(ks+2);

        if (is_prime_big(sum)) {      /* true if sum is prime */
            count++;
            if (count == n)
                return ks;          /* n-th term found */
        }
        k++;                          /* try next integer */
    }
}
