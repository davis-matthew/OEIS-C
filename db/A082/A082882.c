/*
 * A082882  Number of distinct values of A075860(j) when j runs through
 *          composite numbers between n-th and (n+1)-th primes. That is, the
 *          counts of different fixed-points[=prime] reached by iteration of
 *          function A008472(=sum of prime factors) initiated with composite
 *          values between two consecutive primes.
 * https://oeis.org/A082882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*------------------------------------------------------------*/
/*  AEIS sequence A082882:  Number of distinct A075860 fixed  */
/*  points reached from composite numbers between two       */
/*  consecutive primes.                                      */
/*  Formula (OEIS): a(n) = Card(Union(A075860(x))) for          */
/*  x = 1+p(n), …, p(n+1)‑1.                                 */
/*------------------------------------------------------------*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- CONFIGURATION ---------- */
#define LIMIT 200000U                 /* enough for n up to ≈10 000 */

/* ---------- GLOBAL DATA ---------- */
static bool    is_prime_arr[LIMIT + 1];
static uint32_t spf_arr[LIMIT + 1];
static uint32_t *primes = NULL;
static size_t   prime_cnt = 0;

static uint32_t *fixed_arr = NULL;
static bool *seen_arr = NULL;
static uint32_t *seen_list = NULL;
static size_t seen_list_cap = 0;
static size_t seen_list_len = 0;

/* cache of already computed a(n) */
static seq_t *cached_a = NULL;
static size_t cache_size = 0;
static size_t cached_upto = 0;

/* ---------- INITIALISATION ---------- */
static void init_prime_data(void)
{
    /* 1. sieve for primes and smallest prime factor */
    for (uint32_t i = 0; i <= LIMIT; ++i) {
        is_prime_arr[i] = true;
        spf_arr[i] = 0;
    }
    is_prime_arr[0] = is_prime_arr[1] = false;

    for (uint32_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime_arr[i]) {
            for (uint32_t j = i * i; j <= LIMIT; j += i) {
                is_prime_arr[j] = false;
            }
        }
    }

    primes = (uint32_t*)malloc((LIMIT / 10 + 100) * sizeof(uint32_t));
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (is_prime_arr[i]) {
            primes[prime_cnt++] = i;
        }
    }

    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (is_prime_arr[i]) {
            for (uint32_t j = i; j <= LIMIT; j += i) {
                if (spf_arr[j] == 0) spf_arr[j] = i;
            }
        }
    }

    /* 2. cache of fixed points: primes are fixed points */
    fixed_arr = (uint32_t*)calloc(LIMIT + 1, sizeof(uint32_t));
    for (size_t i = 0; i < prime_cnt; ++i) fixed_arr[primes[i]] = primes[i];

    /* 3. bitmap to remember seen fixed points within a call */
    seen_arr = (bool*)calloc(LIMIT + 1, sizeof(bool));
    seen_list_cap = prime_cnt + 10;
    seen_list = (uint32_t*)malloc(seen_list_cap * sizeof(uint32_t));
}

/* ---------- HELPER: sum of distinct prime factors (spf based) ---------- */
static uint32_t sum_distinct_pf(uint32_t n)
{
    uint32_t sum = 0, last = 0;
    while (n > 1) {
        uint32_t p = spf_arr[n];
        if (p != last) { sum += p; last = p; }
        while (n % p == 0) n /= p;
    }
    return sum;
}

/* ---------- HELPER: fixed point of the iteration of sum of primes ---------- */
static uint32_t get_fixed(uint32_t n)
{
    if (n <= 1) return 0;
    if (is_prime_arr[n]) return n;
    uint32_t f = fixed_arr[n];
    if (f != 0) return f;
    f = get_fixed(sum_distinct_pf(n));
    fixed_arr[n] = f;          /* memoise */
    return f;
}

/* ---------- CACHE UTILITIES ---------- */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;              /* 1‑based array */
    if (need > cache_size) {
        size_t new_size = need > 2 * cache_size ? need : 2 * cache_size;
        seq_t *tmp = (seq_t*)realloc(cached_a, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cached_a = tmp;
        cache_size = new_size;
    }
}

/* ---------- COMPUTE a(n) for a specific n (n≥2) ---------- */
static seq_t compute_a(idx_t n)
{
    /* primes are 1‑based: p(1)=primes[0] */
    if ((size_t)n >= prime_cnt) abort();   /* our LIMIT too small */

    uint32_t p_prev = primes[(size_t)n - 1];
    uint32_t p_next = primes[(size_t)n];
    uint32_t start = p_prev + 1;
    uint32_t end   = p_next - 1;
    if (start > end) return 0;             /* no composites */

    size_t count = 0;
    seen_list_len = 0;

    for (uint32_t val = start; val <= end; ++val) {
        if (is_prime_arr[val]) continue;   /* skip primes */
        uint32_t fp = get_fixed(val);
        if (fp == 0) continue;             /* safety */
        if (!seen_arr[fp]) {
            seen_arr[fp] = true;
            if (seen_list_len == seen_list_cap) {
                seen_list_cap *= 2;
                uint32_t *tmp = (uint32_t*)realloc(seen_list,
                                                    seen_list_cap * sizeof(uint32_t));
                if (!tmp) abort();
                seen_list = tmp;
            }
            seen_list[seen_list_len++] = fp;
            ++count;
        }
    }

    /* reset seen bitmap for next call */
    for (size_t i = 0; i < seen_list_len; ++i) seen_arr[seen_list[i]] = false;

    return (seq_t)count;
}

/* ---------- PUBLIC FUNCTION a(idx_t n) ---------- */
seq_t a(idx_t n)
{
    static bool initialized = false;

    if (!initialized) {
        init_prime_data();

        cache_size = 3;
        cached_a   = (seq_t*)calloc(cache_size, sizeof(seq_t));
        if (!cached_a) abort();
        cached_a[0] = 0;      /* unused */
        cached_a[1] = 0;      /* a(1) = 0 */
        cached_upto = 1;
        initialized = true;
    }

    if (n < 1) return 0;          /* undefined below offset */

    ensure_cache(n);

    if (cached_upto >= (size_t)n) return cached_a[(size_t)n];

    for (size_t i = cached_upto + 1; i <= (size_t)n; ++i) {
        cached_a[i] = compute_a((idx_t)i);
    }
    cached_upto = (size_t)n;

    return cached_a[(size_t)n];
}
