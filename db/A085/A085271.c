/*
 * A085271  Difference between n-th composite number and its smallest prime
 *          divisor.
 * https://oeis.org/A085271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A085271:  a(n) = A002808(n) – its smallest prime divisor.
   a(1) = 2, a(2) = 4, a(3) = 6, …  (difference between the nth composite
   number and its smallest prime factor).  */

/* ----- Standard headers ----------------------------------------------------- */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----- Integer square root (for 64‑bit numbers) ----------------------------- */
static uint64_t int_sqrt(uint64_t n)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 32;   /* sqrt(2^64−1) < 2^32 */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid == 0 || mid <= n / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* ----- Prime generator ------------------------------------------------------ */
/* List of primes found so far */
static uint64_t *prime_list   = NULL;
static size_t    prime_cap   = 0;
static size_t    prime_cnt   = 0;
static uint64_t  primes_last_checked = 1;

/* Grow the primes array */
static void grow_prime_cap(void)
{
    size_t new_cap = (prime_cap == 0) ? 1 : prime_cap << 1;
    /* cast necessary for C++ */
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    prime_list = tmp;
    prime_cap  = new_cap;
}

/* Ensure that all primes ≤ limit are in prime_list */
static void ensure_primes_upto(uint64_t limit)
{
    if (prime_cnt == 0) {
        grow_prime_cap();
        prime_list[prime_cnt++] = 2;
        primes_last_checked = 2;
    }
    for (uint64_t n = primes_last_checked + 1; n <= limit; ++n) {
        uint64_t r = int_sqrt(n);
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt && prime_list[i] <= r; ++i) {
            if (n % prime_list[i] == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (prime_cnt == prime_cap) grow_prime_cap();
            prime_list[prime_cnt++] = n;
        }
    }
    primes_last_checked = limit;
}

/* Test primality using the prime list */
static bool is_prime_n(uint64_t n)
{
    if (n < 2) return false;
    uint64_t r = int_sqrt(n);
    ensure_primes_upto(r);
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= r; ++i) {
        if (n % prime_list[i] == 0) return false;
    }
    return true;
}

/* ----- Smallest prime factor of a composite number ------------------------ */
static uint64_t smallest_prime_factor(uint64_t n)
{
    uint64_t r = int_sqrt(n);
    ensure_primes_upto(r);
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= r; ++i) {
        if (n % prime_list[i] == 0) return prime_list[i];
    }
    return n; /* should not happen for composites */
}

/* ----- Cached composites and their smallest prime factors ------------------ */
static seq_t *composites   = NULL;
static seq_t *spf_cache   = NULL;
static size_t composite_cap = 0;
static idx_t  composites_len = 0;
static uint64_t composite_candidate = 4; /* next integer to test for composite */

/* Grow the composites array */
static void grow_composite_cap(void)
{
    size_t new_cap = (composite_cap == 0) ? 1 : composite_cap << 1;
    seq_t *c_tmp = (seq_t *)realloc(composites, new_cap * sizeof(seq_t));
    seq_t *s_tmp = (seq_t *)realloc(spf_cache, new_cap * sizeof(seq_t));
    if (!c_tmp || !s_tmp) abort();
    composites = c_tmp;
    spf_cache = s_tmp;
    composite_cap = new_cap;
}

/* Pre‑compute composites up to the required count */
static void precompute_composites(idx_t needed)
{
    while (composites_len < needed) {
        uint64_t val = composite_candidate++;
        if (!is_prime_n(val)) {
            if (composites_len == composite_cap) grow_composite_cap();
            composites[composites_len]   = (seq_t)val;
            spf_cache[composites_len]   = (seq_t)smallest_prime_factor(val);
            composites_len++;
        }
    }
}

/* --------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined, guard against misuse */
    precompute_composites(n);
    seq_t c   = composites[n - 1];
    seq_t spf = spf_cache[n - 1];
    return c - spf;
}
