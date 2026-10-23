/*
 * A156037  Largest nonprime < n-th prime.
 * https://oeis.org/A156037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest non‑prime < the n‑th prime. (searches downward
 *            from the n‑th prime until a composite or 1 is found.)
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 9289, u64 n <= 9289,
 *            u128 n <= 9289, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* static state for prime generation */
static seq_t *primes = NULL;   /* array of primes found so far */
static idx_t  prime_count = 0; /* number of primes stored */
static size_t prime_cap = 0;   /* current capacity of the array */
static seq_t  next_candidate = 2; /* next integer to test for primality */

/* Ensure that primes[] contains at least n primes. */
static void
ensure_primes(idx_t n)
{
    if (prime_count >= n)
        return;

    if (!primes) {
        prime_cap = 64;
        primes = (seq_t *)malloc(prime_cap * sizeof(seq_t));
        if (!primes)
            return; /* allocation failed; caller will fail gracefully */
    }

    while (prime_count < n) {
        seq_t cand = next_candidate;
        bool is_prime = true;

        /* test divisibility by previously found primes */
        for (idx_t i = 0; i < prime_count; ++i) {
            seq_t p = primes[i];
            if (p * p > cand)
                break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* add to prime list */
            if (prime_count == prime_cap) {
                prime_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(primes, prime_cap * sizeof(seq_t));
                if (!tmp)
                    return; /* allocation failed */
                primes = tmp;
            }
            primes[prime_count++] = cand;
        }

        /* advance to next candidate */
        if (cand == 2)
            next_candidate = 3;
        else
            next_candidate += 2;
    }
}

/* Helper: test whether x is prime using the cached prime list.
   Assumes primes[] contains all primes <= sqrt(x). */
static bool
is_prime_cached(seq_t x)
{
    if (x < 2)
        return false;
    for (idx_t i = 0; i < prime_count; ++i) {
        seq_t p = primes[i];
        if (p * p > x)
            break;
        if (x % p == 0)
            return false;
    }
    return true;
}

/* public function required by the framework */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined offset, return 0 */

    ensure_primes(n);
    if (prime_count < n)
        return (seq_t)0;   /* safety, should not happen */

    seq_t p = primes[n - 1];   /* n-th prime */
    seq_t k = p - 1;           /* start just below the prime */

    /* Search downward for the largest non‑prime. */
    while (true) {
        if (!is_prime_cached(k))
            return k;
        if (k == 0) /* safety */
            return k;
        --k;
    }
}
