/*
 * A098767  Deficient numbers whose reversal is also a deficient number.
 * https://oeis.org/A098767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = the n-th deficient number whose decimal reversal is also
 *            deficient
 * Range:     u8 n <= 171, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Static storage for divisor sums, deficiency flags and cache of sequence values */
static unsigned long long *sigma = NULL;   /* sigma[i] = sum of divisors of i   */
static bool *deficient = NULL;             /* deficient[i] = true if i is deficient  */
static size_t current_limit = 0;           /* current maximum n for which sigma is computed */

static unsigned long long *cache = NULL;   /* cache of a(n) values */
static size_t cache_len  = 0;              /* number of terms stored    */
static size_t cache_cap  = 0;              /* capacity of the cache     */

static unsigned long long next_candidate = 1;   /* next natural number to test */

/* ---------- Helper functions ---------- */

/* Recompute sigma and deficient arrays up to 'limit' (inclusive) */
static void recompute_sigma(size_t limit) {
    unsigned long long *new_sigma = (unsigned long long *)malloc((limit + 1) * sizeof(unsigned long long));
    bool *new_def = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!new_sigma || !new_def) exit(1);

    for (size_t i = 0; i <= limit; ++i) new_sigma[i] = 0;

    for (size_t i = 1; i <= limit; ++i) {
        for (size_t j = i; j <= limit; j += i) {
            new_sigma[j] += i;
        }
    }

    for (size_t i = 1; i <= limit; ++i) {
        new_def[i] = new_sigma[i] < 2 * i;
    }

    free(sigma);
    free(deficient);
    sigma = new_sigma;
    deficient = new_def;
    current_limit = limit;
}

/* Ensure sigma array is computed at least up to 'min_limit' */
static void ensure_limit(size_t min_limit) {
    if (min_limit <= current_limit) return;
    size_t new_limit = current_limit ? current_limit << 1 : 128;
    while (new_limit < min_limit) new_limit <<= 1;
    recompute_sigma(new_limit);
}

/* Reverse decimal digits of x (no leading zeros). */
static unsigned long long reverse_uint64(unsigned long long x) {
    unsigned long long rev = 0;
    while (x > 0) {
        rev = rev * 10 + (x % 10);
        x /= 10;
    }
    return rev;
}

/* Extend cache array to hold at least 'min_size' entries */
static void grow_cache(size_t min_size) {
    if (cache_cap >= min_size) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < min_size) new_cap <<= 1;
    unsigned long long *tmp = (unsigned long long *)realloc(cache, new_cap * sizeof(unsigned long long));
    if (!tmp) exit(1);
    cache = tmp;
    cache_cap = new_cap;
}

/* Check whether a number x > current_limit is deficient using trial factorisation.
   If x <= current_limit, use the precomputed flag. */
static bool deficient_trial(unsigned long long x) {
    if (x <= current_limit) return deficient[(size_t)x];

    unsigned long long sum = 1;          /* sigma(1) = 1 */
    unsigned long long n = x;
    unsigned long long two_n = 2 * x;

    /* Factor 2 separately for efficiency */
    if (n % 2 == 0) {
        unsigned long long part = 1, p_pow = 1;
        while (n % 2 == 0) {
            n /= 2;
            p_pow <<= 1;                 /* multiply by 2 */
            part += p_pow;
        }
        if (sum > two_n / part) return false;
        sum *= part;
    }

    for (unsigned long long p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            unsigned long long part = 1, p_pow = 1;
            while (n % p == 0) {
                n /= p;
                p_pow *= p;
                part += p_pow;
            }
            if (sum > two_n / part) return false;
            sum *= part;
        }
    }
    if (n > 1) {                /* remaining prime factor */
        sum *= (1 + n);
    }
    return sum < two_n;
}

/* ---------- Main sequence function ---------- */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined, but return 0 */

    while (cache_len < (size_t)n) {
        unsigned long long cand = next_candidate;
        ensure_limit(cand);       /* ensure sigma up to cand */

        bool def1 = deficient[(size_t)cand];

        unsigned long long rev = reverse_uint64(cand);
        bool def2;
        if (rev <= current_limit) {
            def2 = deficient[(size_t)rev];
        } else {
            def2 = deficient_trial(rev);
        }

        if (def1 && def2) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = cand;
        }
        ++next_candidate;
    }
    return (seq_t)cache[n - 1];
}
