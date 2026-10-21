/*
 * A066540  The first of two consecutive primes with equal digital sums.
 * https://oeis.org/A066540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 214, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A066540:
   a(n) = first of two consecutive primes with equal decimal digit sums.
   (O(n) search using deterministic Miller‑Rabin for 64‑bit primes.) */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* ---------- Digit sum for a seq_t value ---------- */
static seq_t digit_sum_seq(seq_t x)
{
    seq_t sum = 0;
    while (x > 0) {
        sum += x % 10;
        x   /= 10;
    }
    return sum;
}

/* ---------- Cache for the sequence values ---------- */
static seq_t *cache        = NULL;
static size_t cache_len    = 0;   /* number of elements in cache    */
static size_t cache_cap    = 0;   /* allocated capacity in cache    */

static void grow_cache(void)
{
    size_t new_cap = cache_cap == 0 ? 16 : cache_cap << 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache      = tmp;
    cache_cap  = new_cap;
}

/* ---------- Deterministic Miller‑Rabin for uint64_t ---------- */
static inline uint64_t mulmod64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (__uint128_t)a * b % mod;
}

static inline uint64_t powmod64(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t r = 1;
    while (d) {
        if (d & 1) r = mulmod64(r, a, mod);
        a = mulmod64(a, a, mod);
        d >>= 1;
    }
    return r;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
        23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = powmod64(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mulmod64(x, x, n);
            if (x == n-1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ---------- State for prime generation ---------- */
static uint64_t last_prime_val = 2;   /* previous prime seen          */
static uint64_t candidate_val  = 3;   /* next odd number to test      */

/* Expand the cache until it contains at least `need` terms. */
static void expand_cache(idx_t need)
{
    while (cache_len < (size_t)need) {
        /* Find next prime. */
        while (!is_prime_uint64(candidate_val)) candidate_val += 2;

        /* candidate_val is prime. Check digit sums. */
        seq_t sum_prev = digit_sum_seq((seq_t)last_prime_val);
        seq_t sum_next = digit_sum_seq((seq_t)candidate_val);
        if (sum_prev == sum_next) {
            if (cache_len == cache_cap) grow_cache();
            cache[cache_len++] = (seq_t)last_prime_val;
        }

        /* Step to the following odd candidate. */
        last_prime_val = candidate_val;
        candidate_val += 2;
    }
}

/* ---------- Public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* defined behaviour for offset 0 */
    if (cache_len < (size_t)n) expand_cache(n);
    return cache[n - 1];
}
