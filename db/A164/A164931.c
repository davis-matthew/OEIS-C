/*
 * A164931  n times the n-th noncomposite.
 * https://oeis.org/A164931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*A008578(n) ; A008578(1)=1, A008578(n>1)=prime_{n-1}
 * Range:     u8 n <= 10, u16 n <= 109, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/* ---------- internal helpers for prime generation ---------- */

static uint64_t *primes = NULL;
static size_t primes_cnt = 0;
static size_t primes_cap = 0;

static inline uint64_t isqrt_u64(uint64_t x)
{
    uint64_t r = (uint64_t)sqrtl((long double)x);
    while ((r + 1) * (r + 1) <= x) ++r;
    while (r * r > x) --r;
    return r;
}

/* ensure that at least 'want' primes are stored in 'primes' array */
static void ensure_primes(size_t want)
{
    if (primes_cnt >= want) return;
    if (primes == NULL) {
        primes_cap = (want < 16) ? 16 : want;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        primes_cnt = 0;
    } else if (primes_cap < want) {
        primes_cap = (primes_cap * 2 > want) ? primes_cap * 2 : want;
        primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
    }

    uint64_t candidate = (primes_cnt == 0) ? 2 : primes[primes_cnt - 1] + 1;
    if (candidate > 2 && (candidate & 1ULL) == 0) candidate++; /* skip even */

    while (primes_cnt < want) {
        bool is_prime = true;
        uint64_t limit = isqrt_u64(candidate);
        for (size_t i = 0; i < primes_cnt && primes[i] <= limit; ++i) {
            if (candidate % primes[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            primes[primes_cnt++] = candidate;
        }
        candidate += 2; /* test next odd number */
    }
}

static inline uint64_t get_prime_by_index(size_t idx) /* idx is 0‑based */
{
    ensure_primes(idx + 1);
    return primes[idx];
}

/* ---------- sequence cache ---------- */

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void ensure_cache(idx_t n)
{
    if ((size_t)n <= cache_len) return;

    size_t needed = (size_t)n;
    if (cache_cap < needed) {
        cache_cap = (cache_cap * 2 > needed) ? cache_cap * 2 : needed;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    for (size_t m = cache_len; m < needed; ++m) { /* m is 0‑based index; actual n = m+1 */
        idx_t cur = (idx_t)(m + 1);
        seq_t val;
        if (cur == 1) {
            val = 1;
        } else {
            uint64_t noncomp = get_prime_by_index((size_t)(cur - 2)); /* nth noncomposite for cur>=2 */
            val = (seq_t)cur * (seq_t)noncomp;
        }
        cache[m] = val;
    }
    cache_len = needed;
}

/* ---------- public function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for the sequence but handled gracefully */
    ensure_cache(n);
    return cache[(size_t)(n - 1)];
}
