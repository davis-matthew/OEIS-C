/*
 * A139948  Primes of the form 19x^2+4xy+19y^2.
 * https://oeis.org/A139948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime of the form 19x^2 + 4xy + 19y^2 (n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 407, u32 n <= 41, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static const idx_t residues[24] = {
    19, 55, 103, 115, 223, 271, 307, 355,
    451, 475, 523, 535, 559, 727, 859, 871,
    943, 1039, 1063, 1123, 1147, 1279, 1291, 1375
};

static bool allowed[1428] = { false };
static bool allowed_initialized = false;

static void init_allowed(void)
{
    if (!allowed_initialized) {
        for (int i = 0; i < 24; ++i)
            allowed[(size_t)(residues[i] % 1428)] = true;
        allowed_initialized = true;
    }
}

/* Storage for found primes */
#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> primes;
#else
static seq_t *prime_cache = NULL;
static size_t cache_cap   = 0;
static size_t cache_size  = 0;
#endif

/* Candidate number to test next */
static idx_t next_candidate = 2;

#ifdef __cplusplus
static inline void ensure_cache(idx_t n)
{
    init_allowed();
    while (primes.size() < n) {
        while (true) {
            idx_t mod = next_candidate % 1428;
            if (allowed[(size_t)mod] && is_prime_big((seq_t)next_candidate))
                break;
            ++next_candidate;
        }
        primes.push_back((seq_t)next_candidate);
        ++next_candidate;
    }
}
#else
static inline void grow_cache(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                    /* allocation failed */
        prime_cache = tmp;
        cache_cap   = new_cap;
    }
}

static inline void ensure_cache(idx_t n)
{
    init_allowed();
    while (cache_size < n) {
        while (true) {
            idx_t mod = next_candidate % 1428;
            if (allowed[(size_t)mod] && is_prime_big((seq_t)next_candidate))
                break;
            ++next_candidate;
        }
        grow_cache(cache_size + 1);
        prime_cache[cache_size++] = (seq_t)next_candidate;
        ++next_candidate;
    }
}
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* safety for n < 1 */
    ensure_cache(n);
#ifdef __cplusplus
    return primes[n - 1];
#else
    return prime_cache[n - 1];
#endif
}
