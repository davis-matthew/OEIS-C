/*
 * A095117  a(n) = pi(n) + n, where pi(n) = A000720(n) is the number of primes
 *          <= n.
 * https://oeis.org/A095117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n + π(n) ; π(n) = number of primes ≤ n
 * Range:     u8 n <= 209, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation keeps a dynamic list of primes and a table of
 *  cumulative prime counts (π(i)) up to the largest n that has been
 *  requested.  If a subsequent call asks for a larger n the tables are
 *  extended using trial‑division against the primes that are already
 *  known.  This simple strategy is fast enough for n up to ~10 000
 *  and is robust even when the sequence function is called in a
 *  non‑monotonic order.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Static state for memoisation */
static idx_t          current_limit = 0;    /* largest n handled so far   */
static uint64_t       *primes        = NULL;/* list of primes             */
static size_t         primes_cnt    = 0;    /* number of primes           */
static size_t         primes_cap    = 0;    /* allocated capacity         */
static idx_t         *prime_pi      = NULL;/* π(i) values 0 … current_limit   */
static size_t         pi_cap        = 0;    /* allocated capacity for π(i) */


/* Grow primes array to hold at least new_cap elements */
static void grow_primes(size_t new_cap)
{
    if (new_cap <= primes_cap) return;
    primes_cap = new_cap;
    uint64_t *tmp = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
    if (!tmp) { /* allocation failed – future calls will fail gracefully */
        primes_cap = 0;
        primes_cnt = 0;
        primes = NULL;
        return;
    }
    primes = tmp;
}

/* Grow π(i) array to hold at least new_cap elements */
static void grow_pi(size_t new_cap)
{
    if (new_cap <= pi_cap) return;
    pi_cap = new_cap;
    idx_t *tmp = (idx_t *)realloc(prime_pi, pi_cap * sizeof(idx_t));
    if (!tmp) { /* allocation failed – future calls will fail gracefully */
        pi_cap = 0;
        prime_pi = NULL;
        current_limit = 0;
        return;
    }
    prime_pi = tmp;
}

/* Ensure that tables are valid for all n ≤ limit */
static void ensure_limit(idx_t limit)
{
    if (limit <= current_limit) return;     /* already computed */

    /* Resize tables */
    size_t new_cap_pi = (size_t)(limit + 1); /* indices 0 … limit inclusive */
    grow_pi(new_cap_pi);
    grow_primes(limit / 10 + 16);            /* crude initial capacity */

    /* Initialise indices that may not yet exist */
    if (!prime_pi) return; /* allocation failure – cannot proceed */
    if (!primes) return;   /* allocation failure – cannot proceed */

    prime_pi[0] = 0;
    if (new_cap_pi > 1) prime_pi[1] = 0;

    /* Sieve the new range by trial division using existing primes */
    for (size_t n = (size_t)current_limit + 1; n <= (size_t)limit; ++n) {
        int is_prime = 1;
        if (n < 2) {
            is_prime = 0;
        } else {
            for (size_t i = 0; i < primes_cnt; ++i) {
                uint64_t p = primes[i];
                if (p * p > n) break;
                if (n % p == 0) { is_prime = 0; break; }
            }
        }
        if (is_prime) {
            /* store prime */
            if (primes_cnt == primes_cap) {
                grow_primes(primes_cap ? primes_cap * 2 : 16);
                if (!primes) { /* allocation failed – abort further work */
                    prime_pi[n] = prime_pi[n-1];
                    continue;
                }
            }
            primes[primes_cnt++] = (uint64_t)n;
        }
        /* update π(i) */
        prime_pi[n] = prime_pi[n-1] + (is_prime ? 1 : 0);
    }

    current_limit = limit;
}

/* Sequence function */
seq_t a(idx_t n)
{
    ensure_limit(n);
    if (!prime_pi) return (seq_t)n; /* fallback if allocation failed */
    return (seq_t)n + (seq_t)prime_pi[n];
}
