/*
 * A142698  Primes congruent to 53 mod 57.
 * https://oeis.org/A142698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 53 modulo 57
 * Range:     u8 n <= 2, u16 n <= 173, u32 n <= 174, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *prime_cache = NULL;
static size_t cache_cap   = 0;
static size_t cache_len   = 0;
static uint64_t next_k    = 0;          /* index of next candidate (k in 53+57*k) */

/* returns the n-th prime p such that p % 57 == 53   (n >= 1) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range for OEIS offset 1 */

    /* grow cache until at least n primes stored */
    while (cache_len < (size_t)n) {
        /* candidate = 53 + 57*next_k */
        uint64_t cand = 53ULL + 57ULL * next_k;
        ++next_k;

        /* stop if candidate exceeds the maximum representable value of seq_t
           for builtin integer types */
        static bool max_computed   = false;
        static uint64_t max_val64  = UINT64_MAX;
        if (!max_computed) {
            if (sizeof(seq_t) <= sizeof(uint64_t)) {
                seq_t max_seq = (seq_t)-1;
                max_val64 = (uint64_t)max_seq;
            }
            max_computed = true;
        }
        if (cand > max_val64) {
            /* no further primes will fit in seq_t; break to avoid infinite loop */
            break;
        }

        if (is_prime_big((seq_t)cand)) {
            /* ensure capacity (start with 16, double as needed) */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                prime_cache = tmp;
                cache_cap   = new_cap;
            }
            prime_cache[cache_len++] = (seq_t)cand;
        }
    }

    if (cache_len >= (size_t)n) {
        return prime_cache[n - 1];
    }
    /* Fallback for requests beyond reachable range (e.g., small seq_t types) */
    return 0;
}
