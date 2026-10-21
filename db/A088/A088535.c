/*
 * A088535  n representable as x^2+xy+y^2 and such that 3 divides sigma(n).
 * https://oeis.org/A088535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A088535: n representable as x²+xy+y² and 3 | σ(n). */
/* Values are computed on demand and memoised. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Helper: integer square root for 64‑bit values                       */
static uint32_t isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = n < 0xFFFFFFFFull ? n : 0xFFFFFFFFull;
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid * mid <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return (uint32_t)lo;
}

/* ------------------------------------------------------------------ */
/* Prime list – generated incrementally up to a limit                */
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;
static size_t prime_cap   = 0;
static uint32_t prime_limit = 0;          /* largest prime stored */

/* Extend prime list to at least 'limit' (inclusive).                */
static void ensure_primes(uint32_t limit)
{
    if (limit <= prime_limit) return;

    if (prime_limit == 0) {
        prime_limit = 1;
        prime_cap   = 16;
        prime_list  = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
        if (!prime_list) exit(EXIT_FAILURE);
        prime_count = 0;
    }

    for (uint32_t p = prime_limit + 1; p <= limit; ++p) {
        bool prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint32_t q = prime_list[i];
            if ((uint64_t)q * q > p) break;
            if (p % q == 0) { prime = false; break; }
        }
        if (prime) {
            if (prime_count == prime_cap) {
                prime_cap <<= 1;
                prime_list = (uint32_t *)realloc(prime_list, prime_cap * sizeof(uint32_t));
                if (!prime_list) exit(EXIT_FAILURE);
            }
            prime_list[prime_count++] = p;
        }
    }
    prime_limit = limit;
}

/* ------------------------------------------------------------------ */
/* Check if n is representable by x²+xy+y² (Eisenstein norm).         */
static bool is_representable(uint64_t n)
{
    if (n == 0) return false;
    uint64_t x = n;
    for (size_t i = 0; i < prime_count &&
         (uint64_t)prime_list[i] * prime_list[i] <= x; ++i)
    {
        uint32_t p = prime_list[i];
        uint32_t cnt = 0;
        while (x % p == 0) { x /= p; ++cnt; }
        if ((p % 3u) == 2u && (cnt & 1u))
            return false;
    }
    if (x > 1) {                 /* remaining prime factor > sqrt(n) */
        if ((x % 3u) == 2u) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Compute σ(n) mod 3 using prime factorisation.                     */
static unsigned char sigma_mod3(uint64_t n)
{
    uint64_t x = n;
    unsigned char res = 1;                /* multiplicative identity mod 3 */

    for (size_t i = 0; i < prime_count &&
         (uint64_t)prime_list[i] * prime_list[i] <= x; ++i)
    {
        uint32_t p   = prime_list[i];
        uint32_t cnt = 0;
        while (x % p == 0) { x /= p; ++cnt; }

        unsigned char pmod = (unsigned char)(p % 3u);
        unsigned char sum  = 1;          /* 1 + p + p² + … + pᶜ mod 3 */
        unsigned char power = 1;
        for (uint32_t j = 0; j < cnt; ++j) {
            power = (unsigned char)((power * pmod) % 3);
            sum   = (unsigned char)((sum + power) % 3);
        }
        res = (unsigned char)((res * sum) % 3);
    }

    if (x > 1) {                         /* remaining prime factor of exp 1 */
        unsigned char pmod = (unsigned char)(x % 3u);
        unsigned char sum  = (unsigned char)((1 + pmod) % 3);
        res = (unsigned char)((res * sum) % 3);
    }
    return res;
}

/* ------------------------------------------------------------------ */
/* Memoised sequence storage                                            */
static seq_t *seq_cache = NULL;
static size_t seq_len   = 0;
static size_t seq_cap   = 0;

static void append_term(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        seq_cache = tmp;
        seq_cap   = new_cap;
    }
    seq_cache[seq_len++] = val;
}

/* ------------------------------------------------------------------ */
/* Generate terms up to index n (1‑based).                              */
static void ensure_terms(idx_t n)
{
    static idx_t next_candidate = 1;          /* next integer to test */
    for (; seq_len < n; ++next_candidate) {
        uint64_t cand = (uint64_t)next_candidate;

        /* make sure we have primes up to sqrt(cand) */
        uint32_t limit = isqrt_u64(cand) + 1;
        ensure_primes(limit);

        if (is_representable(cand) && sigma_mod3(cand) == 0)
            append_term((seq_t)cand);
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the build system.                       */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for offset 1 */
    ensure_terms(n);
    return seq_cache[n - 1];
}
