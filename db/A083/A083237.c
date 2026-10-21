/*
 * A083237  First order recursion: a(0)=5; a(n)=prime(n)-a(n-1).
 * https://oeis.org/A083237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = prime(n) – a(n‑1) with a(0) = 5. The function lazily
 *            generates primes and memoises a(n) so that any call order or
 *            large n (up to ≈10000) is handled in amortised O(n)
 * Range:     i8 n <= 31, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#ifdef __cplusplus
#include <vector>
#endif

/* ------------------------------------------------------------------
 *  Prime generator – caches primes in order.  Internally the primes
 *  are stored in a dynamic array that grows as needed.  The algorithm
 *  is deterministic and uses trial division by all previously found
 *  primes.  This is fast enough for the n‑range required here.
 * ------------------------------------------------------------------ */

#ifdef __cplusplus
static std::vector<seq_t> prime_list;          /* 0‑based: prime_list[i] = (i+1)-th prime */
#else
static seq_t *prime_list = NULL;
static idx_t  prime_count = 0;                 /* number of primes currently stored */
#endif

static seq_t get_prime(idx_t idx)
{
    if (idx == 0) return (seq_t)0;        /* unused, for safety */

#ifdef __cplusplus
    while (prime_list.size() < idx) {
        seq_t candidate = prime_list.empty() ? (seq_t)2 : prime_list.back() + 1;
        while (true) {
            bool is_prime = true;
            for (idx_t j = 0; j < prime_list.size(); ++j) {
                seq_t p = prime_list[j];
                if (p * p > candidate) break;
                if (candidate % p == 0) { is_prime = false; break; }
            }
            if (is_prime) {
                prime_list.push_back(candidate);
                break;
            }
            ++candidate;
        }
    }
    return prime_list[idx - 1];
#else
    if (prime_list == NULL) {             /* initialise with first prime */
        prime_list = (seq_t*)malloc(sizeof(seq_t));
        prime_list[0] = (seq_t)2;
        prime_count = 1;
    }
    while (prime_count < idx) {
        seq_t candidate = prime_list[prime_count - 1] + 1;
        while (true) {
            bool is_prime = true;
            for (idx_t j = 0; j < prime_count; ++j) {
                seq_t p = prime_list[j];
                if (p * p > candidate) break;
                if (candidate % p == 0) { is_prime = false; break; }
            }
            if (is_prime) {
                prime_list = (seq_t*)realloc(prime_list, sizeof(seq_t) * (prime_count + 1));
                prime_list[prime_count++] = candidate;
                break;
            }
            ++candidate;
        }
    }
    return prime_list[idx - 1];
#endif
}

/* ------------------------------------------------------------------
 *  Sequence cache – a(i) values are stored as they are produced.
 * ------------------------------------------------------------------ */

#ifdef __cplusplus
static std::vector<seq_t> a_cache;          /* a_cache[i] = a(i) */
#else
static seq_t *a_cache = NULL;
static idx_t  a_cache_len = 0;
#endif

seq_t a(idx_t n)
{
    /* Ensure a(0) is initialized */
    if (n == 0) {
#ifdef __cplusplus
        if (a_cache.empty()) a_cache.push_back((seq_t)5);
#else
        if (a_cache == NULL) {
            a_cache = (seq_t*)malloc(sizeof(seq_t));
            a_cache[0] = (seq_t)5;
            a_cache_len = 1;
        }
#endif
        return a_cache[0];
    }

    /* Prime list must contain primes up to index n */
    (void)get_prime(n);

    /* Extend the cache up to n (if needed) */
#ifdef __cplusplus
    size_t current = a_cache.size();
    if (current <= (size_t)n) {
        a_cache.resize((size_t)n + 1);
        if (current == 0) a_cache[0] = (seq_t)5;   /* safety */
        for (idx_t i = (idx_t)current; i <= n; ++i) {
            seq_t p = get_prime((idx_t)i);
            a_cache[i] = p - a_cache[i - 1];
        }
    }
    return a_cache[n];
#else
    if (a_cache_len <= n) {
        a_cache = (seq_t*)realloc(a_cache, sizeof(seq_t) * (n + 1));
        if (a_cache_len == 0) a_cache[0] = (seq_t)5;
        for (idx_t i = a_cache_len; i <= n; ++i) {
            seq_t p = get_prime(i);
            a_cache[i] = p - a_cache[i - 1];
        }
        a_cache_len = n + 1;
    }
    return a_cache[n];
#endif
}
