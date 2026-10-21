/*
 * A072541  List of pairs of numbers (k, k+4), where k-1 and k+3 are primes.
 * https://oeis.org/A072541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 1720, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A072541: 
   a(2*n-1) = k          where k-1 and k+3 are prime, k is even
   a(2*n)   = k + 4
   The sequence lists the pairs (k, k+4). */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* static storage for all qualifying k values */
    static idx_t *ks = NULL;     /* array of k values (even integers) */
    static size_t cap = 0;       /* allocated capacity                */
    static size_t size = 0;      /* number of entries                 */
    static idx_t next = 4;       /* next even candidate to test       */

    /* We need ceil(n/2) different k's to answer the request. */
    idx_t need = (n + 1) / 2;

    /* Generate k's on demand, caching them for future calls. */
    while (size < need) {
        if (isPrime(next - 1) && isPrime(next + 3)) {
            if (size == cap) {          /* grow the array if necessary */
                size_t new_cap = cap ? cap * 2 : 16;
                idx_t *tmp = (idx_t*)realloc(ks, new_cap * sizeof(idx_t));
                if (!tmp) exit(1);     /* allocation failure */
                ks = tmp;
                cap = new_cap;
            }
            ks[size++] = next;           /* store the new k */
        }
        next += 2;                      /* only even numbers can satisfy the condition */
    }

    /* Return the requested term. */
    if (n & 1) {                        /* odd n -> k  */
        return (seq_t)ks[(n - 1) / 2];
    } else {                            /* even n -> k+4 */
        return (seq_t)(ks[(n / 2) - 1] + 4);
    }
}
