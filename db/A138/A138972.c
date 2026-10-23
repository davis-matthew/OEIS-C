/*
 * A138972  Positions of the primes congruent to 1 mod 6 when all primes
 *          except 2 and 3 are listed in order.
 * https://oeis.org/A138972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Positions of the primes ≡ 1 (mod 6) among all primes except 2 and 3.
   a(n) gives the index in the list {5,7,11,13,…} of the n‑th such prime.
   The function builds the list incrementally and keeps a cache of all
   required positions, so it serves any ordering of calls in O(n) time. */

#include <stdlib.h>
#include <stdbool.h>

/* dynamic cache of primes (including 2 and 3) */
static idx_t *prime_cache   = NULL;
static size_t prime_cnt     = 0;
static size_t prime_cap     = 0;

/* dynamic cache of positions of primes ≡ 1 mod 6 */
static idx_t *pos_cache     = NULL;
static size_t pos_cnt       = 0;
static size_t pos_cap       = 0;

/* next candidate for primality check (odd only, ≥ 5) */
static idx_t next_candidate = 5;
/* index of next_candidate in the prime list excluding 2 and 3 */
static idx_t next_incl_index = 0;

/* ensure the prime cache can hold ‘need’ entries */
static void ensure_prime_cap(size_t need)
{
    if (prime_cap < need) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 64;
        while (new_cap < need) new_cap <<= 1;
        void *tmp = realloc(prime_cache, new_cap * sizeof(idx_t));
        prime_cache = (idx_t *)tmp;   /* cast for C++ */
        prime_cap   = new_cap;
    }
}

/* ensure the position cache can hold ‘need’ entries */
static void ensure_pos_cap(size_t need)
{
    if (pos_cap < need) {
        size_t new_cap = pos_cap ? pos_cap * 2 : 64;
        while (new_cap < need) new_cap <<= 1;
        void *tmp = realloc(pos_cache, new_cap * sizeof(idx_t));
        pos_cache = (idx_t *)tmp;   /* cast for C++ */
        pos_cap   = new_cap;
    }
}

/* simple trial division primality test using the cached primes */
static bool is_prime(idx_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_cache[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* external function required by the problem statement */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* safety for non‑positive indices */

    /* one‑time static initialisation */
    if (prime_cnt == 0) {
        ensure_prime_cap(2);
        prime_cache[0] = 2;
        prime_cache[1] = 3;
        prime_cnt = 2;

        next_candidate = 5;
        next_incl_index = 0;

        ensure_pos_cap(64);
        pos_cnt = 0;
    }

    /* generate primes until we have the n‑th desired position */
    while (pos_cnt < (size_t)n) {
        if (is_prime(next_candidate)) {
            /* store new prime */
            ensure_prime_cap(prime_cnt + 1);
            prime_cache[prime_cnt++] = next_candidate;

            /* count it in the list excluding 2 and 3 */
            ++next_incl_index;

            /* remember its position if ≡ 1 (mod 6) */
            if (next_candidate % 6 == 1) {
                ensure_pos_cap(pos_cnt + 1);
                pos_cache[pos_cnt++] = next_incl_index;
            }
        }
        next_candidate += 2;   /* skip even numbers */
    }

    return (seq_t)pos_cache[(size_t)n - 1];
}
