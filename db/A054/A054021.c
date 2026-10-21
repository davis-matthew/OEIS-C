/*
 * A054021  Numbers n such that Chowla's function of n is divisible by the
 *          number of proper divisors of n.
 * https://oeis.org/A054021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th integer k such that (sum of divisors of k
 *            except 1 and k) is divisible by (number of proper divisors of k)
 * Range:     u8 n <= 115, u16 n <= 8564, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses trial division with a precomputed prime list
   and caches previously computed terms. The code works for both C
   (basic unsigned integer seq_t) and C++ (arbitrary precision seq_t). */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <vector>
#endif

/* --- Prime generation --------------------------------------------------- */
#ifdef __cplusplus
static std::vector<uint32_t> primes_vec;
static bool primes_built = false;
#else
static uint32_t *primes_arr = NULL;
static size_t primes_len = 0, primes_cap = 0;
static bool primes_built = false;
#endif

/* Build a list of primes up to LIMIT using a simple sieve. */
static void init_primes(void) {
    if (primes_built) return;

    const uint32_t LIMIT = 500000;          /* large enough for n ≤ 10 000 */
#ifdef __cplusplus
    std::vector<char> sieve(LIMIT + 1, 0);
    for (uint32_t p = 2; p <= LIMIT; ++p) {
        if (!sieve[p]) {
            primes_vec.push_back(p);
            if ((uint64_t)p * p <= LIMIT) {
                for (uint64_t j = (uint64_t)p * p; j <= LIMIT; j += p)
                    sieve[(size_t)j] = 1;
            }
        }
    }
#else
    /* Allocate array to hold primes. */
    primes_cap = 16;
    primes_arr = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    uint8_t *sieve = (uint8_t *)calloc(LIMIT + 1, 1);
    for (uint32_t p = 2; p <= LIMIT; ++p) {
        if (!sieve[p]) {
            if (primes_len == primes_cap) {
                primes_cap <<= 1;
                uint32_t *tmp = (uint32_t *)realloc(primes_arr,
                                                     primes_cap * sizeof(uint32_t));
                if (!tmp) exit(1);
                primes_arr = tmp;
            }
            primes_arr[primes_len++] = p;
            if ((uint64_t)p * p <= LIMIT) {
                for (uint64_t j = (uint64_t)p * p; j <= LIMIT; j += p)
                    sieve[(size_t)j] = 1;
            }
        }
    }
    free(sieve);
#endif
    primes_built = true;
}

/* --- Factorisation and test -------------------------------------------- */
static bool satisfies(uint64_t n) {
    if (n < 2) return false;   /* only n≥2 are tested */

    seq_t sigma = (seq_t)1;          /* product of divisor sums */
    seq_t proper_cnt = (seq_t)1;     /* product of (e+1)   */

    uint64_t temp = n;

#ifdef __cplusplus
    for (size_t i = 0; i < primes_vec.size() && (uint64_t)primes_vec[i] * primes_vec[i] <= temp; ++i) {
#else
    for (size_t i = 0; i < primes_len && (uint64_t)primes_arr[i] * primes_arr[i] <= temp; ++i) {
#endif
        uint64_t p;
#ifdef __cplusplus
        p = primes_vec[i];
#else
        p = primes_arr[i];
#endif
        if (temp % p == 0) {
            uint64_t exp = 0;
            seq_t term = (seq_t)1;   /* sum of powers of p */
            seq_t p_pow = (seq_t)1;
            while (temp % p == 0) {
                temp /= p;
                exp++;
                p_pow *= (seq_t)p;
                term += p_pow;
            }
            sigma *= term;
            proper_cnt *= (seq_t)(exp + 1);
        }
    }

    if (temp > 1) { /* remaining prime factor */
        sigma *= (seq_t)(1 + temp);
        proper_cnt *= (seq_t)2;
    }

    /* proper_cnt holds τ(n). Subtract 1 to get τ(n) - 1. */
    seq_t proper_minus_one = proper_cnt - (seq_t)1;
    seq_t sum_excl = sigma - (seq_t)1 - (seq_t)n;

    return (sum_excl % proper_minus_one) == (seq_t)0;
}

/* --- Term storage (caching) --------------------------------------------- */
#ifdef __cplusplus
static std::vector<seq_t> terms_vec;
#else
static seq_t *terms_arr = NULL;
static size_t terms_len = 0, terms_cap = 0;
static void ensure_terms_cap(void) {
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap == 0 ? 16 : terms_cap << 1;
        seq_t *tmp = (seq_t *)realloc(terms_arr, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        terms_arr = tmp;
        terms_cap = new_cap;
    }
}
#endif

/* Current number being tested.  This static variable is preserved
   across calls so that multiple invocations of a() do not restart
   the search. */
static uint64_t cur_checked = 2;

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;  /* undefined, safe return */

    init_primes();

#ifdef __cplusplus
    while (terms_vec.size() < (size_t)n) {
        if (satisfies(cur_checked)) {
            terms_vec.push_back((seq_t)cur_checked);
        }
        ++cur_checked;
    }
    return terms_vec[(size_t)(n - 1)];
#else
    while (terms_len < (size_t)n) {
        if (satisfies(cur_checked)) {
            ensure_terms_cap();
            terms_arr[terms_len++] = (seq_t)cur_checked;
        }
        ++cur_checked;
    }
    return terms_arr[(size_t)(n - 1)];
#endif
}
