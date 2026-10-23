/*
 * A102647  a(n) = product of the remainders when the n-th prime is divided by
 *          primes up to the (n-1)-st prime.
 * https://oeis.org/A102647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{p | prime(n), q < p, q prime} (p mod q)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 22, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1                                                */

#include <stdint.h>
#include <stdlib.h>

/* ---------- Prime generation (trial division) ---------- */
static uint64_t *prime_list   = NULL;   /* dynamic array of primes       */
static idx_t     prime_count  = 0;     /* number of primes stored      */
static idx_t     prime_cap    = 0;     /* allocated capacity           */
static uint64_t  next_candidate = 2;   /* next number to test for primality */

/* Grow the prime array if needed */
static int
grow_prime_array(void)
{
    idx_t new_cap = prime_cap ? prime_cap * 2 : 16;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!tmp) {
        free(prime_list);
        prime_list = NULL;
        prime_cap  = 0;
        prime_count = 0;
        return 0;
    }
    prime_list = tmp;
    prime_cap  = new_cap;
    return 1;
}

/* Add the next prime number to the list */
static void
add_next_prime(void)
{
    for (;; ++next_candidate) {
        uint64_t cand = next_candidate;
        int is_prime = 1;
        for (idx_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = 0; break; }
        }
        if (is_prime) {
            if (prime_count == prime_cap && !grow_prime_array())
                return;                 /* Allocation failed – give up           */
            prime_list[prime_count++] = cand;
            next_candidate = cand + 1;
            return;
        }
    }
}

/* Ensure at least n primes are available */
static void
ensure_primes(idx_t n)
{
    while (prime_count < n)
        add_next_prime();
}

/* ---------- Sequence cache (optional but speeds repeated calls) ---------- */
static seq_t *cache        = NULL;   /* a(1) stored at cache[0]             */
static idx_t  cache_len    = 0;      /* number of valid entries in cache     */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n<1                     */

    /* Make sure we have the first n primes. */
    ensure_primes(n);

    /* Grow cache if needed and compute missing terms. */
    if (cache_len < n) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp) {
            free(cache);
            cache = NULL;
            cache_len = 0;
            return 0;                /* allocation failure                    */
        }
        cache = tmp;
        for (idx_t k = cache_len; k < n; ++k) {
            seq_t prod = 1;
            seq_t pk   = (seq_t)prime_list[k];   /* k is 0‑based index of n‑th prime  */
            for (idx_t j = 0; j < k; ++j) {
                seq_t rem = pk % (seq_t)prime_list[j];
                prod *= rem;
            }
            cache[k] = prod;
        }
        cache_len = n;
    }
    return cache[n - 1];
}
