/*
 * A074367  (p^2-5)/4 for odd primes p.
 * https://oeis.org/A074367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (p_n^2 - 5) / 4 where p_n is the n‑th odd prime (that is,
 *            the (n+1)-th prime overall, counting 2 as the first)
 * Range:     u8 n <= 10, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes.h"

/* static state for cached primes */
static idx_t   *primes      = NULL;  /* array of primes in ascending order (including 2) */
static size_t  primes_count = 0;     /* number of primes stored */
static idx_t   primes_limit = 0;     /* maximum value sieved so far */

/* ensure that primes[] contains at least (n+1) primes (index 0..n)   */
static void ensure_primes(idx_t n)
{
    /* we require primes_count > n so that primes[n] is valid            */
    if (primes == NULL) {
        idx_t limit = n * 20 + 100;
        build_primes(limit, &primes, &primes_count);
        primes_limit = limit;
        while (primes_count <= (size_t)n) {
            limit *= 2;
            free(primes);
            build_primes(limit, &primes, &primes_count);
            primes_limit = limit;
        }
    } else if (primes_count <= (size_t)n) {
        idx_t limit = primes_limit * 2;
        free(primes);
        build_primes(limit, &primes, &primes_count);
        primes_limit = limit;
        while (primes_count <= (size_t)n) {
            limit *= 2;
            free(primes);
            build_primes(limit, &primes, &primes_count);
            primes_limit = limit;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* undefined for offset positions   */

    /* make sure primes array is large enough          */
    ensure_primes(n);

    /* n is 1‑based: primes[1] = 3, the first odd prime */
    seq_t p_seq = (seq_t)primes[n];
    return ((p_seq * p_seq - (seq_t)5) / (seq_t)4);
}
