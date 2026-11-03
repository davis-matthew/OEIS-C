/*
 * A062679  Numbers such that every divisor (except 1, but including the
 *          number itself) contains the digit 9.
 * https://oeis.org/A062679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the nth integer whose every divisor (except 1) contains
 *            the digit 9
 * Range:     u8 n <= 16, u16 n <= 3140, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9274 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation searches successive integers that contain the digit 9
   and validates that all their divisors contain 9. A cache is used to
   avoid recomputation for arbitrary call order. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* ---------- Trial division factorisation ----------------------------------- */
static void _factor_u64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    *cnt = 0;
    if (n < 2) return;
    /* factor 2 separately */
    if ((n & 1ULL) == 0) {
        pr[*cnt] = 2ULL;
        exp[*cnt] = 0;
        while ((n & 1ULL) == 0) {
            n >>= 1;
            exp[*cnt] += 1;
        }
        (*cnt)++;
    }
    /* odd primes */
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL) {
        if (n % d == 0) {
            pr[*cnt] = d;
            exp[*cnt] = 0;
            while (n % d == 0) {
                n /= d;
                exp[*cnt] += 1;
            }
            (*cnt)++;
        }
    }
    if (n > 1) {
        pr[*cnt] = n;
        exp[*cnt] = 1;
        (*cnt)++;
    }
}

/* Test whether a 64‑bit integer contains the digit '9'. */
static inline bool contains9_uint64(uint64_t x)
{
    if (x == 0) return false;
    while (x > 0) {
        if (x % 10 == 9) return true;
        x /= 10;
    }
    return false;
}

/* Recursively check that every divisor of the number represented by
   the prime factorisation (pr, exp, cnt) contains the digit 9. */
static bool check_all_divs(const uint64_t *pr, const uint64_t *exp,
                           int idx, uint64_t curr, int cnt)
{
    if (idx == cnt) {
        /* curr is a divisor; skip 1, check otherwise. */
        if (curr > 1 && !contains9_uint64(curr))
            return false;
        return true;
    }
    uint64_t p = pr[idx];
    uint64_t val = 1;
    for (int e = 0; e <= exp[idx]; ++e) {
        if (!check_all_divs(pr, exp, idx + 1, curr * val, cnt))
            return false;
        if (e < exp[idx]) {
            if (val > UINT64_MAX / p) break;  /* avoid overflow */
            val *= p;
        }
    }
    return true;
}

/* Check if a candidate satisfies the sequence property. */
static bool is_good_candidate(uint64_t cand)
{
    if (!contains9_uint64(cand))
        return false;                       /* the number itself must contain 9 */

    uint64_t pr[64], exp[64];
    size_t cnt;
    _factor_u64(cand, pr, exp, &cnt);       /* factorisation */

    /* Every prime factor must contain the digit 9. */
    for (size_t i = 0; i < cnt; ++i)
        if (!contains9_uint64(pr[i]))
            return false;

    /* All composite divisors must also contain 9. */
    return check_all_divs(pr, exp, 0, 1, (int)cnt);
}

/* Cache for memoised terms. */
static seq_t *a_cache      = NULL;   /* array of terms a(1), a(2), ... */
static size_t cache_cap    = 0;       /* allocated capacity */
static size_t cache_len    = 0;       /* number of terms stored so far */
static uint64_t last_cand  = 19;      /* next integer to test */

/* Ensure the cache can hold at least 'need' terms. */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *new_p = mem_resize(seq_t, a_cache, new_cap);
    a_cache = new_p;
    cache_cap = new_cap;
}

/* Main sequence routine. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* undefined for indices below offset */

    /* Fast return if already cached. */
    if ((size_t)n <= cache_len)
        return a_cache[n - 1];

    /* Extend cache to at least n terms. */
    ensure_cache((size_t)n + 1); /* some slack */

    /* Generate new terms until we reach n. */
    while (cache_len < (size_t)n) {
        while (true) {
            if (is_good_candidate(last_cand)) {
                a_cache[cache_len++] = (seq_t)last_cand;
                ++last_cand;
                break;
            }
            ++last_cand;
        }
    }
    return a_cache[n - 1];
}
