/*
 * A177465  Primes of the form (35*10^k-71)/9.
 * https://oeis.org/A177465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime of the form (35*10^k - 71)/9
 * Range:     u8 none, u16 n <= 2, u32 n <= 2, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   is_prime_big, mul_small, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/is_prime_big.h"
#include "../utils/mul_small.h"

#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#include <stdbool.h>
#endif

/* Static state for caching computed primes. */
static seq_t pow10_val = 1;   /* 10^current_k */
static idx_t current_k = 0;   /* current exponent of 10 */
static idx_t next_k = 1;      /* next exponent to test */

#ifdef __cplusplus
static std::vector<seq_t> primes;
#else
static seq_t *primes = NULL;
static size_t primes_cap = 0;
static size_t primes_count = 0;
#endif

/* Helper to advance pow10_val to 10^target_k. */
static void advance_pow10(idx_t target_k) {
    while (current_k < target_k) {
        pow10_val = mul_small(pow10_val, 10);
        current_k++;
    }
}

/* Store a new prime into the cache. */
static void store_prime(seq_t val) {
#ifdef __cplusplus
    primes.push_back(val);
#else
    if (primes_count >= primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 1;
        seq_t *tmp = realloc(primes, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        primes = tmp;
        primes_cap = new_cap;
    }
    primes[primes_count++] = val;
#endif
}

/* Small helpers to query cache size/value. */
#ifdef __cplusplus
static inline size_t primes_size(void) { return primes.size(); }
static inline seq_t primes_value_at(size_t idx) { return primes[idx]; }
#else
static inline size_t primes_size(void) { return primes_count; }
static inline seq_t primes_value_at(size_t idx) { return primes[idx]; }
#endif

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* Undefined, but return 0 for safety. */

    /* Generate primes until we have at least n of them. */
    while (primes_size() < (size_t)n) {
        /* Ensure pow10_val equals 10^next_k. */
        advance_pow10(next_k);

        /* Compute candidate: (35 * 10^k - 71) / 9. */
        seq_t candidate = pow10_val * 35;
        candidate -= 71;
        candidate /= 9;

        if (is_prime_big(candidate)) {
            store_prime(candidate);
        }

        next_k++;
    }

    return primes_value_at(n - 1);
}
