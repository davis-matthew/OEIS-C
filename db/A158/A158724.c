/*
 * A158724  Numbers n such that prime(n)^2 + 1 is a semiprime.
 * https://oeis.org/A158724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth index n such that prime(n)^2 + 1 is a semiprime
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-01)
 * Helpers:   build_primes_upto, ipow_i32, is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"
#include "../utils/ipow_i32.h"
#include "../utils/is_prime_seq.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Static cache for primes */
static uint64_t *prime_list   = NULL;
static size_t   prime_cnt    = 0;
static size_t   prime_limit  = 0;

/* Static cache for sequence terms (indices n) */
static idx_t *term_list     = NULL;
static size_t term_cnt       = 0;
static size_t term_cap       = 0;
static idx_t next_candidate = 1;  /* next prime index to test */

/* Ensure we have at least `need` primes in prime_list */
static void ensure_prime_limit(size_t need)
{
    if (prime_cnt >= need)
        return;
    if (prime_limit == 0)
        prime_limit = 2048;

    while (prime_cnt < need) {
        if (prime_limit < need * 2)
            prime_limit = need * 2;

        uint64_t *tmp = NULL;
        size_t    tmpcnt = 0;
        build_primes_upto(prime_limit, &tmp, &tmpcnt);

        free(prime_list);
        prime_list = tmp;
        prime_cnt  = tmpcnt;

        if (prime_cnt < need)
            prime_limit <<= 1;
    }
}

/* Ensure term_list can hold at least `need` entries */
static void ensure_term_cap(size_t need)
{
    if (term_cap >= need)
        return;
    term_cap = term_cap ? term_cap * 2 : 8;
    term_list = (idx_t *)realloc(term_list, term_cap * sizeof(idx_t));
}

/* Test whether v is a semiprime (product of two primes, possibly equal) */
static bool is_semiprime(seq_t v)
{
    if (v < 2)
        return false;

    seq_t sqrt_v = floor_sqrt(v);
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if ((seq_t)p > sqrt_v)
            break;
        if (v % (seq_t)p == 0) {
            seq_t q = v / (seq_t)p;
            return is_prime_seq(q);
        }
    }
    return false;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n < 1; harmless */
        return 0;

    while (term_cnt < (size_t)n) {
        ensure_prime_limit((size_t)next_candidate);

        uint64_t p = prime_list[(size_t)next_candidate - 1];
        seq_t v = (seq_t)p * (seq_t)p + (seq_t)1;

        if (is_semiprime(v)) {
            ensure_term_cap(term_cnt + 1);
            term_list[term_cnt++] = next_candidate;
        }
        ++next_candidate;
    }
    return (seq_t)term_list[(size_t)n - 1];
}
