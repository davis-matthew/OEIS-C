/*
 * A139892  Primes of the form 10x^2+21y^2.
 * https://oeis.org/A139892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime that can be written as 10*x^2 + 21*y^2
 * Range:     u8 n <= 5, u16 n <= 415, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- 64‑bit integer square root ---------- */
static uint64_t isqrt_uint64(uint64_t n)
{
    uint64_t low = 0, high = n >> 1 | 1;          /* high ≥ sqrt(n) */
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        if (mid > n / mid)        /* mid*mid > n, avoid overflow */
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

/* ---------- Prime sieve helpers ---------- */
static uint64_t *prime_list = NULL;      /* dynamic array of primes */
static size_t prime_cnt  = 0;            /* number of primes stored   */
static size_t prime_cap  = 0;            /* allocated capacity       */
static uint64_t last_checked = 1;        /* last integer checked for prime */

/* Ensure that primes up to 'limit' are stored in prime_list. */
static void ensure_primes_up_to(uint64_t limit)
{
    if (limit <= last_checked) return;

    for (uint64_t i = last_checked + 1; i <= limit; ++i) {
        bool is_p = true;
        uint64_t sqrt_i = isqrt_uint64(i);
        for (size_t idx = 0; idx < prime_cnt && prime_list[idx] <= sqrt_i; ++idx) {
            if (i % prime_list[idx] == 0) { is_p = false; break; }
        }
        if (is_p) {
            if (prime_cnt == prime_cap) {
                size_t new_cap = prime_cap ? prime_cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
                if (!tmp) abort();
                prime_list = tmp;
                prime_cap = new_cap;
            }
            prime_list[prime_cnt++] = i;
        }
    }
    last_checked = limit;
}

/* Test whether m is prime using the generated prime list. */
static bool is_prime_uint64(uint64_t m)
{
    if (m < 2) return false;
    uint64_t sqrt_m = isqrt_uint64(m);
    ensure_primes_up_to(sqrt_m);
    for (size_t idx = 0; idx < prime_cnt; ++idx) {
        uint64_t p = prime_list[idx];
        if (p > sqrt_m) break;
        if (m % p == 0) return false;
    }
    return true;
}

/* ---------- Representation check ---------- */
static bool representable(uint64_t p)
{
    uint64_t max_y = isqrt_uint64(p / 21);
    for (uint64_t y = 0; y <= max_y; ++y) {
        uint64_t t = 21ULL * y * y;
        if (t > p) break;
        uint64_t rem = p - t;
        if (rem % 10ULL != 0ULL) continue;
        uint64_t x2 = rem / 10ULL;
        uint64_t x  = isqrt_uint64(x2);
        if (x * x == x2) return true;
    }
    return false;
}

/* ---------- Sequence generation with memoization ---------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t cand = 3;          /* next odd number to test */

/* Grow cache array to at least new_cap elements. */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, safe fallback */

    while (cache_len < (size_t)n) {
        /* Skip composite numbers quickly */
        if (!is_prime_uint64(cand)) {
            cand += 2;
            continue;
        }

        /* Check representability by 10x^2 + 21y^2 */
        if (representable(cand)) {
            grow_cache(cache_len + 16);
            cache[cache_len++] = (seq_t)cand;
        }
        cand += 2;   /* next odd candidate */
    }
    return cache[n - 1];
}
