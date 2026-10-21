/*
 * A061591  a(n) = 2^x, where x = p*(p-1)/2 and p is the n-th prime.
 * https://oeis.org/A061591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(p*(p-1)/2) where p is the n‑th prime
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow, build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/build_primes.h"

/* static cache for primes */
static idx_t *prime_list = NULL;   /* array of primes */
static size_t prime_count = 0;     /* number of primes currently stored */

/* Ensure at least n primes are available */
static void
ensure_primes(idx_t n)
{
    if (prime_count >= (size_t)n)
        return;

    /* approximate upper bound for the n-th prime:
       for n > 5, p_n < n*(log n + log log n) < n*20  (safe for n <= 10000)  */
    idx_t limit = (n > 5) ? n * 20 + 10 : 15;

    /* keep increasing until we have enough primes */
    while (1) {
        if (prime_count > 0) {
            free(prime_list);
            prime_list = NULL;
            prime_count = 0;
        }
        build_primes(limit, &prime_list, &prime_count);
        if (prime_count >= (size_t)n)
            break;
        limit *= 2;   /* safety margin, should not happen for n <= 10000 */
    }
}

seq_t
a(idx_t n)
{
    if (n == 0)         /* the offset is 1, so n<1 is undefined */
        return 0;

    ensure_primes(n);

    /* p = n-th prime, note primes are zero‑based indexed */
    seq_t p = (seq_t)prime_list[n-1];

    /* exponent x = p*(p-1)/2 in seq_t to avoid overflow */
    seq_t exp = (p * (p - (seq_t)1)) / 2;

    return big_pow((seq_t)2, exp);
}
