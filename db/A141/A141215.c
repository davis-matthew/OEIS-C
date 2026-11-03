/*
 * A141215  Primes of the form 3*x^2+5*x*y-3*y^2 (as well as 5*x^2+9*x*y+y^2).
 * https://oeis.org/A141215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 3266, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9472 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

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

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

/* A141215 – primes of the form 3*x^2+5*x*y-3*y^2
 * Sequence is the set of primes p such that either p==61
 * or the Legendre symbol (61|p)=+1.  A prime is added to the cache
 * only once (duplicates are impossible).  The routine generates primes
 * in ascending order and keeps a static cache so that calls with
 * arbitrary n are answered in O(1) after the cache is built up to n. */

static seq_t *prime_cache = NULL;      /* cache of primes in the sequence */
static size_t cache_size = 0;           /* number of cached terms */
static size_t cache_cap   = 0;           /* allocated capacity */

static idx_t next_candidate = 3;        /* next odd integer to test (2 is skipped) */

/* Ensure the cache can store at least `need` elements. */
static void ensure_cache(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    prime_cache = mem_resize(seq_t, prime_cache, new_cap);
    cache_cap = new_cap;
}

/* Return true if p is a prime belonging to the sequence. */
static inline bool in_sequence(idx_t p)
{
    if (p == 61U) return true;          /* the discriminant itself */
    /* For all other odd primes we require (61|p)=+1. */
    return jacobi_symbol(61U, p) == 1;
}

/* Main function requested by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* below offset – behaviour irrelevant */

    ensure_cache((size_t)n);

    /* Generate primes until we have `n` terms in the cache. */
    while (cache_size < (size_t)n)
    {
        idx_t p = next_candidate;
        next_candidate += 2;       /* skip even numbers (p==2 is not in the sequence) */

        if (!is_prime_seq(p) && !is_prime_big((seq_t)p))
            continue;             /* composite – skip */

        if (in_sequence(p))
        {
            prime_cache[cache_size++] = (seq_t)p;
        }
    }

    return prime_cache[n - 1];
}
