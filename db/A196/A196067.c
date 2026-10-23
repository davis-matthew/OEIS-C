/*
 * A196067  Number of pendant vertices in the rooted tree with Matula-Goebel
 *          number n.
 * https://oeis.org/A196067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; a(2) = 2; If n is prime, let t = prime_pi(n) (index of
 *            prime). a(n) = a(t) if t is prime a(n) = a(t) + 1 otherwise If n
 *            is composite (n = r * s , r,s ≥ 2), a(n) = a(r) + a(s) -
 *            (is_prime(r) + is_prime(s))
 * Range:     u8 n <= 110, u16 n <= 110, u32 n <= 110, u64 n <= 110,
 *            u128 n <= 110, big any n
 * Helpers:   _smallest_prime_factor, is_prime, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime.h"          /* is_prime(idx_t) */
#include "../utils/prime_pi.h"          /* prime_pi(idx_t) */
#include "../utils/_smallest_prime_factor.h" /* _smallest_prime_factor(uint64_t) */

static seq_t *cache = NULL;
static bool   *used  = NULL;
static idx_t  cache_cap = 0;

/* Resize the cache and the used flag array to accommodate indices up to n */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap <= n) new_cap <<= 1;

    seq_t  *new_cache = (seq_t *)calloc(new_cap, sizeof(seq_t));
    bool   *new_used  = (bool  *)calloc(new_cap, sizeof(bool ));
    if (cache) {
        memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
        free(cache);
    }
    if (used) {
        memcpy(new_used, used, cache_cap * sizeof(bool));
        free(used);
    }
    cache     = new_cache;
    used      = new_used;
    cache_cap = new_cap;
}

/* Recursive computation with memoization */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* values below the offset are unused */
    ensure_cache(n);
    if (used[n]) return cache[n];

    seq_t result;
    if (n == 1) {
        result = 0;
    } else if (n == 2) {
        result = 2;
    } else if (is_prime(n)) {
        idx_t t = (idx_t)prime_pi(n);           /* index of prime n */
        seq_t a_t = a(t);
        result = is_prime(t) ? a_t : a_t + 1;
    } else {                                 /* composite case */
        uint64_t spf = _smallest_prime_factor(n);
        idx_t r = (idx_t)spf;
        idx_t s = (idx_t)(n / r);
        seq_t a_r = a(r);
        seq_t a_s = a(s);
        seq_t sub = 0;
        if (is_prime(r)) sub += 1;
        if (is_prime(s)) sub += 1;
        result = a_r + a_s - sub;
    }

    cache[n] = result;
    used[n]  = true;
    return result;
}
