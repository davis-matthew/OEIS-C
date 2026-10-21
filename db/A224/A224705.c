/*
 * A224705  Composite numbers n divisible by Omega(n)^2 (the square of the
 *          number of their prime factors, counted with multiplicity).
 * https://oeis.org/A224705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth composite integer divisible by (Omega(n))^2
 * Range:     u8 n <= 14, u16 n <= 2620, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---- integer sqrt for uint64_t ---------------------------------------- */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = n;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        if (mid == 0) { lo = 1; continue; }
        uint64_t div = n / mid;
        if (mid == div) {
            return mid;
        } else if (mid < div) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

/* ---- prime list helper ----------------------------------------------- */
static uint64_t *g_primes = NULL;
static size_t    g_prime_cnt  = 0;
static size_t    g_prime_cap  = 0;
static uint64_t  g_last_checked = 1;

static void ensure_primes(uint64_t limit)
{
    while (g_prime_cnt == 0 || g_primes[g_prime_cnt - 1] < limit) {
        uint64_t candidate = g_last_checked + 1;
        while (true) {
            bool is_p = true;
            uint64_t r = isqrt_u64(candidate);
            for (size_t i = 0; i < g_prime_cnt && g_primes[i] <= r; ++i) {
                if (candidate % g_primes[i] == 0) {
                    is_p = false;
                    break;
                }
            }
            if (is_p) break;
            ++candidate;
        }
        if (g_prime_cnt == g_prime_cap) {
            size_t new_cap = g_prime_cap == 0 ? 32 : g_prime_cap * 2;
            uint64_t *tmp = (uint64_t *)realloc(g_primes, new_cap * sizeof(uint64_t));
            if (!tmp) abort();
            g_primes = tmp;
            g_prime_cap = new_cap;
        }
        g_primes[g_prime_cnt++] = candidate;
        g_last_checked = candidate;
    }
}

/* ---- sequence cache helper ------------------------------------------- */
static seq_t *g_cache = NULL;
static size_t g_cache_sz  = 0;
static size_t g_cache_cap = 0;

static void ensure_cache(size_t needed)
{
    if (g_cache_sz >= needed) return;
    if (needed > g_cache_cap) {
        size_t new_cap = g_cache_cap == 0 ? 32 : g_cache_cap * 2;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(g_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        g_cache = tmp;
        g_cache_cap = new_cap;
    }
}

/* ---- main sequence function ------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* defined only for n>=1 */

    /* Convert to size_t for indexing */
    size_t idx = (size_t)n;

    ensure_cache(idx);

    /* generate terms until we have at least idx terms cached */
    static uint64_t g_next = 2;   /* next integer to test, starts at 2 */

    while (g_cache_sz < idx) {
        uint64_t val = g_next++;
        if (val < 4) continue; /* 1,2,3 are not composite */

        uint64_t temp = val;
        uint64_t omega = 0;

        ensure_primes(isqrt_u64(temp));

        for (size_t i = 0; i < g_prime_cnt && g_primes[i] * g_primes[i] <= temp; ++i) {
            uint64_t p = g_primes[i];
            if (temp % p == 0) {
                do {
                    temp /= p;
                    ++omega;
                } while (temp % p == 0);
            }
        }
        if (temp > 1) {      /* remaining large prime factor */
            ++omega;
        }

        if (omega > 1) {     /* composite only */
            uint64_t square = omega * omega;
            if (val % square == 0) {
                g_cache[g_cache_sz++] = (seq_t)val;
            }
        }
    }

    return g_cache[idx - 1];
}
