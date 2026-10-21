/*
 * A230077  Table a(n,m) giving in row n all k from {1, 2, ..., prime(n)-1}
 *          for which the Legendre symbol (k/prime(n)) = +1, for odd prime(n).
 * https://oeis.org/A230077
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 3029, u16 n <= 9990, u32 n <= 9990, u64 n <= 9990,
 *            u128 n <= 9990, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A230077: a(n) is the flattening of m² mod p(n),
   where p(n) is the n‑th prime and m runs from 1 to (p(n)-1)/2.
   The sequence is indexed so that a(2) is the first term.
   For n ≥ 2 the value is computed as
   let k be the smallest index with
        ∑_{i=2}^{k} (p(i)-1)/2  >  n-2,
   let m = (n-2) - ∑_{i=2}^{k-1} (p(i)-1)/2 + 1,
   then a(n) = m² mod p(k).
*/

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* below offset is undefined, return 0 as a placeholder */
    if (n < 2)
        return 0;

    /* index into the flattened table, starting at 0 for a(2) */
    idx_t idx = n - 2;

    /* iterate through primes starting with prime(2)=3 */
    idx_t k = 2;               /* 1‑based prime index (prime(2)=3) */

    while (1) {
        uint32_t p = get_prime(k);            /* current prime p(k) */
        idx_t  len = (p - 1) / 2;             /* number of terms in this row */

        if (idx < len) {
            /* the desired term lies in this row */
            uint64_t m = (uint64_t)(idx + 1); /* 1‑based m */
            uint64_t mod = (m * m) % (uint64_t)p;
            return (seq_t)mod;                /* cast to return type */
        }

        idx -= len;   /* skip this row */
        k++;          /* move to next prime */
    }
}
