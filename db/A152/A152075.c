/*
 * A152075  a(n) = the smallest prime p > prime(n) such that p - prime(n) is
 *          squarefree, where prime(n) is the n-th prime.
 * https://oeis.org/A152075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest prime p > prime(n) such that p - prime(n) is
 *            squarefree
 * Range:     u8 n <= 53, u16 n <= 6541, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_squarefree, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/next_prime.h"
#include "../utils/is_squarefree.h"

/*  n  (idx_t)  >= 1
    returns a(n) as seq_t
*/
seq_t a(idx_t n) {
    /* Obtain the n-th prime.  get_prime returns a 32‑bit value; cast to uint64 for safety. */
    uint64_t pn = (uint64_t)get_prime(n);

    /* Search for the next prime whose difference from pn is squarefree. */
    uint64_t p = next_prime(pn);
    while (1) {
        seq_t diff = (seq_t)p - (seq_t)pn;
        if (is_squarefree(diff))
            return (seq_t)p;
        p = next_prime(p);
    }
}
