/*
 * A129480  a(n) = Prime(17*n).
 * https://oeis.org/A129480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Prime(17*n) (offset 1)
 * Range:     u8 n <= 3, u16 n <= 384, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <math.h>
#include "../utils/sieve_primes.h"

static idx_t *prime_cache = NULL;   /* cached primes, 0‑indexed */
static size_t prime_count = 0;      /* number of cached primes */

/* conservative upper bound for the n-th prime (n >= 1) */
static size_t prime_upper_bound(idx_t n)
{
    if (n == 0) return 2;
    if (n < 6)  return 15;   /* p_5 = 13, p_6 = 17 */
    double dn   = (double)n;
    double ln   = log(dn);
    double lnln = log(ln);
    double bound = dn * (ln + lnln) + 3.0;   /* +3 gives margin */
    return (size_t)bound;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* below offset, dummy */

    idx_t target = 17 * n;                /* 17*n‑th prime needed */

    /* rebuild sieve if we don't have enough primes cached */
    if (prime_count < (size_t)target) {
        size_t limit = prime_upper_bound(target);

        if (prime_cache != NULL) {
            free(prime_cache);
            prime_cache = NULL;
        }
        sieve_primes(limit, &prime_cache, &prime_count);
    }

    /* prime_cache is 0‑indexed: prime_cache[0] = 2 */
    return (seq_t)prime_cache[target - 1];
}
