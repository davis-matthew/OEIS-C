/*
 * A065154  Numbers for which the cototient of the totient is strictly less
 *          than the totient of the cototient.
 * https://oeis.org/A065154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A065154 – Numbers n such that cototient(φ(n)) < φ(cototient(n)).
   The function computes the n‑th such number by scanning integers,
   caching φ values, and storing the found terms.
   The implementation works for both the C and the C++ (arbitrary
   precision) build. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#ifdef __cplusplus
#  include <vector>
#endif

/* --------------------------------------------------------------------
   Prime sieve (fixed upper bound – more than enough for n ≤ 10000).
   The list of primes is shared by all invocations, so we build it
   once and keep it in static storage.
   -------------------------------------------------------------------- */
static uint32_t *prime_list = NULL;
static size_t   primes_cnt  = 0;
#define PRIME_LIMIT 200000

static void init_primes(void)
{
    static bool done = false;
    if (done) return;
    bool *is_composite = (bool*)malloc(PRIME_LIMIT+1);
    memset(is_composite, 0, PRIME_LIMIT+1);
    uint32_t tmp[20000];
    size_t tmp_cnt = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_composite[i]) {
            tmp[tmp_cnt++] = i;
            if ((uint64_t)i*i <= PRIME_LIMIT) {
                for (uint64_t j = (uint64_t)i*i; j <= PRIME_LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }
    primes_cnt  = tmp_cnt;
    prime_list  = (uint32_t*)malloc(primes_cnt * sizeof(uint32_t));
    memcpy(prime_list, tmp, primes_cnt * sizeof(uint32_t));
    free(is_composite);
    done = true;
}

/* --------------------------------------------------------------------
   Cached φ values.
   We keep an array that grows on demand.  Index 0 is unused, φ(1)=1.
   -------------------------------------------------------------------- */
static uint64_t *phi_cache = NULL;
static size_t    cache_size = 0;      /* number of valid entries          */

static void ensure_phi_cache(size_t needed)
{
    if (needed < cache_size) return;
    size_t new_size = needed + 1;
    uint64_t *new_cache = (uint64_t*)realloc(phi_cache, new_size * sizeof(uint64_t));
    if (!new_cache) exit(EXIT_FAILURE);
    /* initialise new entries to zero */
    for (size_t i = cache_size; i < new_size; ++i) new_cache[i] = 0;
    phi_cache  = new_cache;
    cache_size = new_size;
}

/* --------------------------------------------------------------------
   Euler's totient via trial division using the prime list.
   Cached results are returned immediately.
   -------------------------------------------------------------------- */
static uint64_t phi_of(uint64_t n)
{
    if (n < cache_size && phi_cache[n]) return phi_cache[n];
    uint64_t result = n;
    uint64_t t = n;
    for (size_t i = 0; i < primes_cnt && (uint64_t)prime_list[i]*prime_list[i] <= t; ++i) {
        uint32_t p = prime_list[i];
        if (t % p == 0) {
            while (t % p == 0) t /= p;
            result = result / p * (p - 1);
        }
    }
    if (t > 1) result = result / t * (t - 1);
    if (n < cache_size) phi_cache[n] = result;
    return result;
}

/* --------------------------------------------------------------------
   Sequence storage – different for C and for C++ (big‑int) builds.
   -------------------------------------------------------------------- */
#ifdef __cplusplus
static std::vector<seq_t> terms_vec;   /* automatically ready to use   */
#else
static seq_t *terms_arr = NULL;
static size_t terms_cap = 0;           /* allocated capacity            */
static size_t terms_count = 0;         /* number of terms stored        */
#endif

/* --------------------------------------------------------------------
   Main function – returns the n‑th term of A065154.
   -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!prime_list) init_primes();

#ifndef __cplusplus
    /* ensure terms array exists */
    if (!terms_arr) {
        terms_cap = 16;
        terms_arr = (seq_t*)malloc(terms_cap * sizeof(seq_t));
        if (!terms_arr) exit(EXIT_FAILURE);
    }
#endif

    const idx_t target = n;
    const idx_t min_n = (idx_t)1;      /* offset is 1 */
    static uint64_t last_tested = min_n;  /* last integer examined */

    /* grow the stored sequence until we have at least target elements */
    while (true) {
#ifdef __cplusplus
        if ((idx_t)terms_vec.size() >= target) break;
#else
        if (terms_count >= target) break;
#endif
        ++last_tested;
        ensure_phi_cache(last_tested);          /* expand φ cache if needed */

        uint64_t phi_n          = phi_of(last_tested);
        uint64_t cot_phi_n      = phi_n - phi_of(phi_n);

        uint64_t n_minus_phi    = last_tested - phi_n;
        uint64_t phi_n_minus_phi = phi_of(n_minus_phi);

        if (cot_phi_n < phi_n_minus_phi) {
#ifdef __cplusplus
            terms_vec.push_back((seq_t)last_tested);
#else
            if (terms_count == terms_cap) {  /* grow array */
                size_t new_cap = terms_cap == 0 ? 16 : terms_cap * 2;
                seq_t *tmp = (seq_t*)realloc(terms_arr, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                terms_arr = tmp;
                terms_cap = new_cap;
            }
            terms_arr[terms_count++] = (seq_t)last_tested;
#endif
        }
    }

#ifdef __cplusplus
    return terms_vec[target-1];
#else
    return terms_arr[target-1];
#endif
}
