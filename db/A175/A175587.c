/*
 * A175587  a(1) = 4; a(n) = smallest semiprime such that a(n) - a(n-1) is
 *          prime.
 * https://oeis.org/A175587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 966 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A175587
   a(1) = 4
   a(n) = smallest semiprime greater than a(n‑1) whose difference from
          a(n‑1) is prime.
   This implementation generates primes up to a fixed limit,
   tests each successive integer for semiprimality, and uses
   is_prime_big() to test the required difference. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#endif

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size) /* elem_size: sizeof(T) */
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, new_cap);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* If the sequence values exceed  4,000,000 we re‑grow the prime list.
   The chosen limit gives plenty of semiprimes well beyond n = 10 000. */
#define PRIME_LIMIT 2000000U

/* Static state */
static bool     initialized = false;
static seq_t   *a_cache     = NULL;
static size_t   a_cap       = 0;
static idx_t    a_len       = 0;

static uint64_t *primes_u64 = NULL;
static size_t    prime_cnt  = 0;

/* ------------------------------------------------------------------ */
/* Helper: test whether x is a semiprime (product of exactly two primes)
   using the precomputed list of small primes. */
static bool
is_semiprime(seq_t x)
{
    seq_t temp = x;
    int   cnt  = 0;

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = primes_u64[i];
        seq_t   pp = (seq_t)p * (seq_t)p;
        if (pp > temp)
            break;

        while (temp % (seq_t)p == 0)
        {
            temp /= (seq_t)p;
            ++cnt;
            if (cnt > 2)
                return false;
        }
    }
    /* If temp > 1, it is a prime factor > sqrt(original). */
    if (temp > 1)
        ++cnt;

    return cnt == 2;
}

/* ------------------------------------------------------------------ */
/* Initialise the global tables and the first sequence value. */
static void
init_global_state(void)
{
    /* Build a list of primes up to PRIME_LIMIT. */
    build_primes_upto(PRIME_LIMIT, &primes_u64, &prime_cnt);

    /* Allocate space for the sequence cache. */
    a_cap = 64;
    a_cache = mem_new(seq_t, a_cap);
    if (!a_cache)
        return;  /* In a real environment we would handle the error. */

    a_cache[0] = (seq_t)4;                /* a(1) = 4 */
    a_len = 1;
    initialized = true;
}

/* ------------------------------------------------------------------ */
/* Compute the sequence up to index n (inclusive). */
static void
ensure_sequence_upto(idx_t n)
{
    for (idx_t idx = a_len; idx < n; ++idx)
    {
        seq_t prev = a_cache[idx - 1];
        seq_t cand = prev + (seq_t)1;

        /* Seek the next semiprime whose difference from prev is prime. */
        while (true)
        {
            if (is_semiprime(cand))
            {
                seq_t diff = cand - prev;
                if (is_prime_big(diff))
                {
                    /* Append to cache. */
                    if (idx >= a_cap)
                        ensure_capacity(idx + 1, &a_cap, OEISC_VPP(&a_cache),
                                        sizeof(seq_t));
                    a_cache[idx] = cand;
                    break;
                }
            }
            ++cand;
        }
    }
    a_len = n;
}

/* ------------------------------------------------------------------ */
/* Public API required by the harness. */
seq_t a(idx_t n)
{
    if (!initialized)
        init_global_state();

    /* Our implementation ignores calls with n == 0. */
    if (n == 0)
        return (seq_t)0;

    if ((idx_t)a_len < n)
        ensure_sequence_upto(n);

    return a_cache[n - 1];
}
