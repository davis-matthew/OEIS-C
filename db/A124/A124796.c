/*
 * A124796  Coefficients in expansion of powers of the operator
 *          "multiplication by f(x) followed by differentiation", in the prime
 *          factorization order.
 * https://oeis.org/A124796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is given by the recurrence on the primes‑factorisation * of
 *            n. For n = \prod_i p_i^{k_i} (p_0=2,p_1=3,p_2=5,…) * let k =
 *            [k0,k1,k2,…]. Then * * a(k) = a([k0-1,k1,…]) (k0>0) * +
 *            (k0+1)*a([k0,k1-1,…]) (k1>0) * + Σ_{i≥2}
 *            (k_{i-1}+1)*a([k0-1,k1,…,k_{i-2}, * k_{i-1}+1,k_i-1,k_{i+1},…])
 *            (k_i>0) *...
 * Range:     u8 n <= 215, u16 n <= 4319, u32 n <= 9177, u64 n <= 9177,
 *            u128 n <= 9177, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *spf_arr      = NULL;   /* smallest prime factor */
static size_t    spf_limit    = 0;
static bool      spf_init     = false;

static uint64_t *prime_arr    = NULL;   /* list of primes up to current limit */
static size_t    prime_cnt    = 0;
static bool      primes_init  = false;

static int *prime_index_arr = NULL;     /* prime → index in prime_arr */
static size_t prime_index_limit = 0;

/* cache of computed a(n) values */
static seq_t  *cache_vals   = NULL;
static bool   *cache_valid  = NULL;
static size_t  cache_limit  = 0;

/* ---------- small‑prime‑factor table ---------- */
static void init_spf(size_t limit)
{
    if (spf_init && spf_limit >= limit) return;
    free(spf_arr);
    spf_arr = (uint64_t*)calloc(limit + 1, sizeof(uint64_t));
    for (size_t i = 2; i <= limit; ++i) {
        if (spf_arr[i] == 0) {                /* i is prime */
            spf_arr[i] = i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    if (spf_arr[j] == 0) spf_arr[j] = i;
            }
        }
    }
    spf_limit = limit;
    spf_init  = true;
}

/* ---------- prime list ---------- */
static void init_primes(size_t limit)
{
    if (primes_init && prime_cnt > 0) {
        if (limit <= (size_t)prime_arr[prime_cnt-1]) return;
    }
    if (prime_arr) free(prime_arr);
    if (!spf_init) init_spf(limit);

    size_t c = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (spf_arr[i] == i) ++c;
    prime_arr = (uint64_t*)malloc(c * sizeof(uint64_t));
    size_t pos = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (spf_arr[i] == i) prime_arr[pos++] = i;
    prime_cnt = c;
    primes_init = true;
}

/* ---------- prime index table ---------- */
static void init_prime_index(size_t limit)
{
    if (prime_index_arr && prime_index_limit >= limit) return;
    free(prime_index_arr);
    prime_index_arr = (int*)malloc((limit + 1) * sizeof(int));
    for (size_t i = 0; i <= limit; ++i) prime_index_arr[i] = -1;
    for (size_t i = 0; i < prime_cnt; ++i)
        prime_index_arr[prime_arr[i]] = (int)i;
    prime_index_limit = limit;
}

static void ensure_globals(idx_t n)
{
    init_spf((size_t)n);
    init_primes((size_t)n);
    init_prime_index((size_t)n);
}

static void ensure_cache(idx_t n)
{
    if (cache_limit > (size_t)n) return;
    size_t new_limit = (size_t)n + 1;
    seq_t *new_vals = (seq_t*)realloc(cache_vals, new_limit * sizeof(seq_t));
    bool *new_valid = (bool*)realloc(cache_valid, new_limit * sizeof(bool));
    for (size_t i = cache_limit; i < new_limit; ++i) new_valid[i] = false;
    cache_vals = new_vals;
    cache_valid = new_valid;
    cache_limit = new_limit;
}

/* ---------- recursive evaluation ---------- */
static seq_t eval_a(idx_t n)
{
    if (n == 1) return 1;
    ensure_cache(n);
    if (cache_valid[n]) return cache_vals[n];

    /* factorise n using SPF table */
    uint64_t tmp = n;
    uint32_t *exps = (uint32_t*)malloc(prime_cnt * sizeof(uint32_t));
    for (size_t i = 0; i < prime_cnt; ++i) exps[i] = 0;
    while (tmp > 1) {
        uint64_t p = spf_arr[tmp];
        uint32_t e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        int idx = prime_index_arr[p];
        exps[idx] = e;
    }

    uint32_t e0 = (prime_cnt > 0) ? exps[0] : 0;
    uint32_t e1 = (prime_cnt > 1) ? exps[1] : 0;

    seq_t res = 0;

    if (e0 > 0)
        res += eval_a(n / 2);

    if (e1 > 0)
        res += (seq_t)(e0 + 1) * eval_a(n / 3);

    for (size_t i = 2; i < prime_cnt; ++i) {
        if (exps[i] > 0 && e0 > 0) {
            seq_t coeff = (seq_t)(exps[i-1] + 1);
            idx_t nxt = n / prime_arr[i];
            nxt = nxt / 2 * prime_arr[i-1];
            res += coeff * eval_a(nxt);
        }
    }

    free(exps);
    cache_vals[n] = res;
    cache_valid[n] = true;
    return res;
}

/* ---------- public API ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n < 1 */
    ensure_globals(n);
    return eval_a(n);
}
