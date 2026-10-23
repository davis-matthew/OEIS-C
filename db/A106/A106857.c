/*
 * A106857  Primes of the form x^2+xy+3y^2, with x and y nonnegative.
 * https://oeis.org/A106857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime of the form x^2 + x*y + 3*y^2 with
 *            non‑negative integers x and y, enumerated in increasing order
 * Range:     u8 n <= 19, u16 n <= 2654, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#ifdef __cplusplus
#include <vector>
#endif

/* ---------- fast modular arithmetic (used by is_prime) ---------- */
static inline uint64_t mod_mul64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)(((__int128)a * b) % mod);
}

static uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp) {
        if (exp & 1) result = mod_mul64(result, base, mod);
        base = mod_mul64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* ---------- deterministic Miller‑Rabin for 64‑bit integers ---------- */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] =
        {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mod_mul64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ---------- helper comparator for qsort ---------- */
static int cmp_u64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    if (va < vb) return -1;
    return va > vb;
}

/* ---------- C++ implementation ---------- */
#ifdef __cplusplus
static std::vector<seq_t> primes;       /* sorted list of primes */
static uint64_t current_limit = 0;      /* current upper bound used in regeneration */

/* regenerate full list of primes of the form x^2+xy+3y^2 up to current_limit */
static void regenerate_primes()
{
    /* first gather all candidate values */
    std::vector<uint64_t> cand;
    for (uint64_t y = 0; 3 * y * y <= current_limit; ++y) {
        uint64_t three_y2 = 3 * y * y;
        uint64_t remaining = current_limit - three_y2;
        uint64_t discr = y * y + 4ULL * remaining;
        uint64_t sqrt_d = (uint64_t)sqrt((long double)discr);
        while ((sqrt_d + 1) * (sqrt_d + 1) <= discr) ++sqrt_d;
        while (sqrt_d * sqrt_d > discr) --sqrt_d;
        uint64_t xmax = (sqrt_d > y) ? (sqrt_d - y) / 2 : 0;
        for (uint64_t x = 0; x <= xmax; ++x) {
            unsigned __int128 val128 = (__int128)x * x + (__int128)x * y + (__int128)3 * y * y;
            uint64_t val = (uint64_t)val128;
            if (val >= 3) cand.push_back(val);
        }
    }

    std::sort(cand.begin(), cand.end());
    cand.erase(std::unique(cand.begin(), cand.end()), cand.end());

    primes.clear();
    for (uint64_t v : cand) {
        if (is_prime_uint64(v))
            primes.push_back((seq_t)v);
    }
}

static void ensure_primes(idx_t n)
{
    if (primes.size() < n) {
        if (current_limit == 0) current_limit = 1000;
        while (primes.size() < n) {
            current_limit <<= 1;       /* double the limit */
            regenerate_primes();
        }
    }
}
#endif

/* ---------- C implementation ---------- */
#ifndef __cplusplus
static uint64_t *prime_list = NULL; /* array of primes as uint64_t */
static idx_t prime_count = 0;
static idx_t prime_cap   = 0;
static uint64_t cur_limit = 0;

/* ensure that prime_list has capacity for at least 'need' entries */
static void grow_cap(idx_t need)
{
    if (prime_cap < need) {
        idx_t new_cap = prime_cap ? prime_cap * 2 : 16;
        while (new_cap < need) new_cap <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        prime_list = tmp;
        prime_cap  = new_cap;
    }
}

/* regenerate full list up to cur_limit */
static void regenerate_primes_c()
{
    /* collect all candidate values */
    uint64_t *cand = NULL;
    size_t cand_cap = 0;
    size_t cand_cnt = 0;

    for (uint64_t y = 0; 3 * y * y <= cur_limit; ++y) {
        uint64_t three_y2 = 3 * y * y;
        uint64_t remaining = cur_limit - three_y2;
        uint64_t discr = y * y + 4ULL * remaining;
        uint64_t sqrt_d = (uint64_t)sqrt((long double)discr);
        while ((sqrt_d + 1) * (sqrt_d + 1) <= discr) ++sqrt_d;
        while (sqrt_d * sqrt_d > discr) --sqrt_d;
        uint64_t xmax = (sqrt_d > y) ? (sqrt_d - y) / 2 : 0;
        for (uint64_t x = 0; x <= xmax; ++x) {
            unsigned __int128 val128 = (__int128)x * x + (__int128)x * y + (__int128)3 * y * y;
            uint64_t val = (uint64_t)val128;
            if (val >= 3) {
                if (cand_cnt == cand_cap) {
                    cand_cap = cand_cap ? cand_cap * 2 : 256;
                    uint64_t *tmp = (uint64_t *)realloc(cand, cand_cap * sizeof(uint64_t));
                    cand = tmp;
                }
                cand[cand_cnt++] = val;
            }
        }
    }

    if (cand_cnt == 0) return;

    qsort(cand, cand_cnt, sizeof(uint64_t), cmp_u64);

    /* keep only unique values in place */
    size_t uniq = 0;
    for (size_t i = 0; i < cand_cnt; ++i) {
        if (i == 0 || cand[i] != cand[i-1]) {
            cand[uniq++] = cand[i];
        }
    }

    /* filter primes */
    grow_cap(uniq);
    prime_count = 0;
    for (size_t i = 0; i < uniq; ++i) {
        if (is_prime_uint64(cand[i]))
            prime_list[prime_count++] = cand[i];
    }

    free(cand);
}

static void ensure_primes_c(idx_t n)
{
    if (prime_count < n) {
        if (cur_limit == 0) cur_limit = 1000;
        while (prime_count < n) {
            cur_limit <<= 1;          /* double the limit */
            regenerate_primes_c();
        }
    }
}
#endif

/* ---------- requested function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

#ifdef __cplusplus
    ensure_primes(n);
    return primes[n - 1];
#else
    ensure_primes_c(n);
    return (seq_t)prime_list[n - 1];
#endif
}
