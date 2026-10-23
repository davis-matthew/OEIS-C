/*
 * A104294  a(n) = prime((prime(n)+1)/2).
 * https://oeis.org/A104294
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime((prime(n)+1)/2) (defined for n ≥ 2)
 * Range:     u8 n <= 28, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Dynamic list of primes.  The list is grown lazily as required. */
static seq_t *prime_array = NULL;
static size_t prime_capacity = 0;
static size_t prime_count    = 0;   /* number of primes stored */
static idx_t  next_candidate = 3;   /* next odd number to test for primality */

/* Allocate or expand the prime array as necessary.
   The caller must keep the array after it is filled. */
static void ensure_prime_count(idx_t k)
{
    /* initialise the array with the first prime 2 */
    if (prime_count == 0) {
        prime_capacity = 16;
        prime_array = (seq_t*)malloc(prime_capacity * sizeof(seq_t));
        if (!prime_array) exit(EXIT_FAILURE);
        prime_array[prime_count++] = (seq_t)2;
    }

    /* store more primes until we have at least k of them */
    while (prime_count < k) {
        if (is_prime((uint64_t)next_candidate)) {
            if (prime_count == prime_capacity) {
                prime_capacity *= 2;
                seq_t *tmp = (seq_t*)realloc(prime_array,
                                             prime_capacity * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                prime_array = tmp;
            }
            prime_array[prime_count++] = (seq_t)next_candidate;
        }
        next_candidate += 2;          /* skip even numbers */
    }
}

/* OEIS entry A104294.  For n < 2 the value is undefined; we return 0. */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;      /* offset 2 – a(2) is the first term */

    /* prime(n) */
    ensure_prime_count(n);
    seq_t pn = prime_array[n-1];

    /* (prime(n)+1)/2  →  index of the next prime to return */
    seq_t mid_seq = (pn + (seq_t)1) / (seq_t)2;
    idx_t mid     = (idx_t)mid_seq;   /* convert to an integer index */

    /* prime((prime(n)+1)/2) */
    ensure_prime_count(mid);
    return prime_array[mid-1];
}
