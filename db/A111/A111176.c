/*
 * A111176  Sophie Germain 4-almost primes.
 * https://oeis.org/A111176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the nth integer that is a 4‑almost prime (exactly four
 *            prime factors counted with multiplicity) and for which 2·a(n)+1
 *            is also a 4‑almost prime
 * Range:     u8 n <= 2, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Compute floor(sqrt(x)) for a 64‑bit integer without overflow. */
static uint32_t isqrt_u64(uint64_t x)
{
    uint32_t low = 0, high = (uint32_t)UINT32_MAX, mid, best = 0;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        if ((__uint128_t)mid * mid <= x) {
            best = mid;
            low  = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return best;
}

/* -------------------------------------------------------------------------
 * Dynamic prime list
 * ------------------------------------------------------------------------- */
static uint32_t *prime_list = NULL;
static size_t   prime_cap   = 0;
static size_t   prime_cnt   = 0;

static void add_prime(uint32_t p)
{
    if (prime_cnt == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 64;
        uint32_t *tmp = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
        if (!tmp) return;          /* graceful OOM – future calls will fail */
        prime_list = tmp;
        prime_cap = new_cap;
    }
    prime_list[prime_cnt++] = p;
}

static bool is_prime_uint32(uint32_t n)
{
    if (n < 2) return false;
    uint32_t root = isqrt_u64(n);
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= root; ++i) {
        if (n % prime_list[i] == 0) return false;
    }
    return true;
}

static void ensure_primes_upto(uint32_t limit)
{
    if (prime_cnt && prime_list[prime_cnt-1] >= limit) return;
    uint32_t start = prime_cnt ? prime_list[prime_cnt-1] + 1 : 2;
    for (uint32_t x = start; x <= limit; ++x) {
        if (is_prime_uint32(x)) add_prime(x);
    }
}

/* -------------------------------------------------------------------------
 * Count of prime factors with multiplicity (omega function)
 * ------------------------------------------------------------------------- */
static unsigned count_omega_uint64(uint64_t x)
{
    unsigned cnt = 0;
    uint32_t limit = isqrt_u64(x);
    ensure_primes_upto(limit);
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= limit; ++i) {
        uint32_t p = prime_list[i];
        while (x % p == 0) {
            if (++cnt > 4) return cnt;   /* early exit */
            x /= p;
            if (x == 1) break;
        }
        limit = isqrt_u64(x);   /* shrink limit as x shrinks */
    }
    if (x > 1) ++cnt;           /* remaining prime factor */
    return cnt;
}

/* -------------------------------------------------------------------------
 * Term storage
 * ------------------------------------------------------------------------- */
static seq_t *term_list = NULL;    /* array of seq_t values */
static size_t term_cap   = 0;
static size_t term_cnt   = 0;

static void push_term(seq_t val)
{
    if (term_cnt == term_cap) {
        size_t new_cap = term_cap ? term_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(term_list, new_cap * sizeof(seq_t));
        if (!tmp) return;          /* graceful OOM */
        term_list = tmp;
        term_cap = new_cap;
    }
    term_list[term_cnt++] = val;
}

/* -------------------------------------------------------------------------
 * Search for the sequence terms
 * ------------------------------------------------------------------------- */
static uint64_t current_candidate = 2;   /* integer to test next */

static void generate_terms(size_t target)
{
    while (term_cnt < target) {
        uint64_t p = current_candidate;

        if (count_omega_uint64(p) == 4) {
            __uint128_t dbl128 = (__uint128_t)2 * p + 1;
            if (dbl128 <= UINT64_MAX) {
                uint64_t q = (uint64_t)dbl128;
                if (count_omega_uint64(q) == 4) {
                    push_term((seq_t)p);
                }
            }
        }
        ++current_candidate;
    }
}

/* -------------------------------------------------------------------------
 * Public function
 * ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined – harmless */
    generate_terms((size_t)n);
    return term_list[n - 1];
}
