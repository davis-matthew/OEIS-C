/*
 * A145199  Nonsquarefree numbers k such that k+1 is prime.
 * https://oeis.org/A145199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n‑th integer k such that k is not squarefree and k+1
 *            is prime
 * Range:     u8 n <= 30, u16 n <= 4071, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/is_squarefree.h"

/* Return the n‑th element of A145199.
 *   n : 1‑based index (n >= 1)                                 */
seq_t a(idx_t n)
{
    idx_t found = 0;
    idx_t p = 2;

    /* iterate over the integers, testing for primes and square‑freeness of p‑1 */
    while (found < n) {
        if (is_prime(p)) {
            seq_t k = (seq_t)p - (seq_t)1;      /* k = p−1 */
            if (!is_squarefree(k)) {
                ++found;
                if (found == n)
                    return k;
            }
        }
        ++p;
    }

    /* Should never be reached for n >= 1 */
    return (seq_t)0;
}
