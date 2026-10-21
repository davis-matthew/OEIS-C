/*
 * A206626  Primes p such that p + previousprime(p) is a squarefree number.
 * https://oeis.org/A206626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   is_squarefree, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes p such that p + previousprime(p) is squarefree. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_squarefree.h"
#include "../utils/next_prime.h"

/* a(1) = 3, a(2) = 17, a(3) = 23, ... */
seq_t a(idx_t n)
{
    /* iterate primes in increasing order, keeping track of the
       previous prime to test the square‑freedom condition. */
    uint64_t prev = 2;      /* previous prime */
    uint64_t curr = 3;      /* current prime candidate */
    idx_t   count = 0;      /* how many valid primes found so far */

    while (true) {
        /* test whether p + previousprime(p) is square‑free. */
        seq_t sum = (seq_t)curr + (seq_t)prev;
        if (is_squarefree(sum)) {
            count++;
            if (count == n)
                return (seq_t)curr;   /* found the n‑th term */
        }

        /* move to the next prime.  next_prime returns the smallest
           prime strictly greater than its argument. */
        prev = curr;
        curr = next_prime(prev);
    }
}
