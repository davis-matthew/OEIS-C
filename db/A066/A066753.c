/*
 * A066753  a(n) = least natural number k such that n + Sum_{i=1..k} prime(i)
 *          is prime if such k exists; = 0 otherwise.
 * https://oeis.org/A066753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the OEIS A066753 sequence:
   a(n) = least k≥1 such that n + Σ_{i=1..k} prime(i) is prime.
   If no such k can be found within a practical bound, a(n) = 0. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/sieve_primes.h"

static idx_t *prime_list = NULL;   /* primes stored in ascending order */
static size_t prime_cnt = 0;       /* number of primes currently stored */
static idx_t prime_lim = 0;        /* limit used for the current sieve */

/* Ensure that we have at least `need` primes in prime_list.  */
static void ensure_primes(size_t need)
{
    if (prime_cnt >= need)
        return;

    /* Start with a reasonable sieve size and double as necessary. */
    idx_t limit = (prime_lim == 0) ? 1000000 : prime_lim * 2;
    while (prime_cnt < need) {
        /* Free any previous allocation before generating a new sieve. */
        if (prime_list) {
            free(prime_list);
            prime_list = NULL;
            prime_cnt = 0;
        }
        sieve_primes(limit, &prime_list, &prime_cnt);
        if (prime_cnt >= need) {
            prime_lim = limit;
            break;
        }
        /* Double the limit and try again. */
        limit = limit * 2;
    }
}

/* Return a(n) for the sequence A066753.  */
seq_t a(idx_t n)
{
    if (n == 0)            /* defensive handling of the offset root case */
        return 0;

    /* Running sum of the first k primes. */
    idx_t sum = 0;
    size_t k   = 0;        /* count of primes used so far */

    /* In practice, we search until a prime is found.  If the search
       exceeds a reasonable bound (1 000 000 primes), we return 0.
       The bound is generous for all requested n up to 10 000. */
    const size_t MAX_K = 1000000;
    while (k < MAX_K) {
        /* Ensure the prime_list contains at least the (k+1)-st prime. */
        ensure_primes(k + 1);
        sum += prime_list[k];
        ++k;

        if (is_prime(n + sum))
            return (seq_t)k;   /* least k found */
    }

    /* No prime found within the search bound.  Return 0. */
    return 0;
}
