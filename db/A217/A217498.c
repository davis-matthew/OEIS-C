/*
 * A217498  Primes of the form 2*n^2 + 58*n + 27.
 * https://oeis.org/A217498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 29, u32 n <= 32, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute primes of the form 2*k^2 + 58*k + 27.
   Return the n‑th such prime (OEIS index starts at 1). */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* static state used to memoise previously found primes     */
static seq_t *prime_cache   = NULL; /* array of primes in order  */
static size_t prime_len    = 0;      /* number of primes stored   */
static size_t prime_cap    = 0;      /* capacity of prime_cache   */
static uint64_t next_k     = 0;      /* next k value to test      */

/* Return the n‑th prime of the quadratic 2*k^2 + 58*k + 27 */
seq_t a(idx_t n)
{
    if (n == 0)                          /* undefined for 0, return 0   */
        return (seq_t)0;

    while (prime_len < n) {              /* generate more primes  */
        /* compute 2*k^2 + 58*k + 27 in seq_t arithmetic */
        seq_t k  = (seq_t)next_k;                            /* cast once */
        seq_t val = ((seq_t)2 * k * k) + ((seq_t)58 * k) + (seq_t)27;

        if (is_prime_big(val)) {          /* check primality     */
            if (prime_cap == 0) {          /* first allocation   */
                prime_cap = 16;
                prime_cache = (seq_t*)malloc(prime_cap * sizeof(seq_t));
            } else if (prime_len >= prime_cap) { /* grow            */
                prime_cap *= 2;
                prime_cache = (seq_t*)realloc(prime_cache,
                                              prime_cap * sizeof(seq_t));
            }
            prime_cache[prime_len++] = val;
        }
        ++next_k;                         /* advance to next k   */
    }
    return prime_cache[n - 1];            /* return stored prime */
}
