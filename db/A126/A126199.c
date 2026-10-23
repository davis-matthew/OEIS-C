/*
 * A126199  a(n) = prime(n)*prime(n+1) + prime(n) + prime(n+1).
 * https://oeis.org/A126199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A126199 – a(n) = prime(n)*prime(n+1) + prime(n) + prime(n+1)
    (All primes are the 1‑based sequence of primes starting with 2,3,5,...)
*/

#include <stdlib.h>
#include <stdint.h>

/* Helper: compute the k‑th prime (k ≥ 1).  The routine keeps a static
 * cached list of primes and extends it only as needed.  The primes are
 * stored in a dynamically allocated array that grows as required.
 */
static uint32_t *cached_primes = NULL;   /* array of primes */
static size_t    cached_cnt   = 0;       /* number of primes cached */
static uint32_t  next_candidate = 3;     /* next odd number to test */

static uint32_t kth_prime(idx_t k)
{
    if (k == 0) return 0;               /* not used for k=0 */

    if (cached_cnt == 0) {              /* first call – initialise */
        cached_primes = NULL;
        cached_cnt    = 0;
        /* store the first prime 2 */
        cached_primes = (uint32_t *)realloc(cached_primes, sizeof(uint32_t));
        cached_primes[0] = 2;
        cached_cnt = 1;
        next_candidate = 3;
    }

    /* Grow the cache until we have at least k primes */
    while (cached_cnt < (size_t)k) {
        uint32_t cand = next_candidate;
        int is_prime = 1;
        /* trial division by cached primes up to sqrt(cand) */
        for (size_t i = 0; i < cached_cnt; ++i) {
            uint32_t p = cached_primes[i];
            if ((uint64_t)p * p > cand) break;
            if (cand % p == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            cached_primes = (uint32_t *)realloc(cached_primes,
                                                (cached_cnt + 1) * sizeof(uint32_t));
            cached_primes[cached_cnt++] = cand;
        }
        next_candidate += 2;            /* only odd candidates after 2 */
    }
    return cached_primes[k - 1];
}

/*  Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* offset is 1; behaviour for n=0 is irrelevant */

    seq_t p = (seq_t)kth_prime(n);      /* prime(n)   */
    seq_t q = (seq_t)kth_prime(n + 1);  /* prime(n+1) */

    return p * q + p + q;               /* p*q + p + q */
}
