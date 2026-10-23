/*
 * A142634  Primes congruent to 47 mod 55.
 * https://oeis.org/A142634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 164, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th prime congruent to 47 modulo 55 (offset 1). */

#include <stdlib.h>
#include <stdbool.h>

/* Test whether num is prime using trial division with the pre‑computed
   list of small primes.  num is assumed to be small enough that its
   square fits in idx_t. */
static bool is_prime_candidate(idx_t num, const size_t *primes, size_t count) {
    if (num < 2)
        return false;
    for (size_t i = 0; i < count; ++i) {
        idx_t p = (idx_t)primes[i];
        if (p * p > num)
            break;
        if (num % p == 0)
            return false;
    }
    return true;
}

seq_t a(idx_t n) {
    if (n == 0)
        return 0;   /* behaviour for n < 1 is irrelevant */

    /* --- static state that survives across calls --- */
    static seq_t *cache = NULL;      /* stored primes ≡ 47 (mod 55) */
    static size_t cache_len = 0;     /* number of stored primes       */
    static size_t cache_cap = 0;     /* capacity of *cache            */
    static idx_t candidate = 47;     /* next number ≡ 47 (mod 55) to test */

    /* --- small primes for trial division ----------------- */
    static bool *small_prime_flag = NULL;   /* sieve flags up to sqrt_limit */
    static size_t *small_primes = NULL;     /* list of those primes */
    static size_t small_prime_count = 0;    /* number of small primes */

    /* The maximum candidate for n≈10000 is below 3 000 000;
       sqrt(3 000 000) < 2000, we use 4000 as a safe bound. */
    const size_t sqrt_limit = 4000;

    /* Initialise small primes, once */
    if (small_prime_count == 0) {
        small_prime_flag = (bool *)calloc(sqrt_limit + 1, sizeof(bool));
        if (!small_prime_flag)
            return 0;   /* allocation failure – fallback */

        small_prime_flag[0] = small_prime_flag[1] = true;
        for (size_t i = 2; i * i <= sqrt_limit; ++i)
            if (!small_prime_flag[i])
                for (size_t j = i * i; j <= sqrt_limit; j += i)
                    small_prime_flag[j] = true;

        /* Count primes and allocate array */
        for (size_t i = 2; i <= sqrt_limit; ++i)
            if (!small_prime_flag[i])
                ++small_prime_count;

        small_primes = (size_t *)malloc(small_prime_count * sizeof(size_t));
        if (!small_primes) {
            free(small_prime_flag);
            return 0;
        }
        size_t idx = 0;
        for (size_t i = 2; i <= sqrt_limit; ++i)
            if (!small_prime_flag[i])
                small_primes[idx++] = i;
        /* candidate already starts at 47 */
    }

    /* Fill cache up to n primes */
    while (cache_len < (size_t)n) {
        if (is_prime_candidate(candidate, small_primes, small_prime_count)) {
            /* Grow cache if necessary */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap == 0 ? 32 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* allocation failure – return 0 as safe fallback */
                    free(small_prime_flag);
                    free(small_primes);
                    free(cache);
                    return 0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)candidate;
        }
        candidate += 55;   /* next number ≡ 47 (mod 55) */
    }

    return cache[n - 1];
}
