/*
 * A188673  a(n) = smallest semiprime such that prime(n) = a(n) - p, p prime.
 * https://oeis.org/A188673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   count_prime_factors, get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A188673
   a(n) = smallest semiprime s such that prime(n) = s - p for some prime p
*/

#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime.h"
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    /* Get the n-th prime.  The helper returns a seq_t that fits into the
       current build‑time seq_t type, so we cast it to idx_t for arithmetic. */
    seq_t pn_val = get_prime(n);
    idx_t pn = (idx_t)pn_val;

    /* Search upward from the next integer after the prime.  For each candidate
       s we check whether s - pn is prime and whether s itself has exactly
       two prime factors (counting multiplicity). */
    for (idx_t s = pn + 1;; ++s) {
        idx_t diff = s - pn;
        if (!is_prime(diff))
            continue;                     /* difference not prime */

        /* Count prime factors of s; a semiprime has exactly two. */
        if ((idx_t)count_prime_factors((seq_t)s) == 2)
            return (seq_t)s;               /* found the smallest one */
    }
}
