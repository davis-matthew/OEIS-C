/*
 * A079130  Primes such that iterated sum-of-digits (A038194) is a square.
 * https://oeis.org/A079130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A079130: prime p with iterated sum‑of‑digits a square.
 *  Equivalent condition: p ≡ 1 or 4 (mod 9).  The function finds the n‑th
 *  such prime by iteratively testing successive odd candidates with a
 *  simple trial‑division primality test.  Results are memoised across
 *  calls for efficiency. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static uint64_t *primes_u64   = NULL; /* array of all primes found so far */
static size_t    prime_cnt    = 0;
static size_t    prime_cap    = 0;
static uint64_t *qual_u64      = NULL; /* array of qualifying primes (mod 9 == 1 or 4) */
static size_t    qual_cnt     = 0;
static size_t    qual_cap     = 0;
static uint64_t  candidate    = 3;     /* next odd candidate to test */
static bool      initialized  = false;

static void ensure_prime_capacity(void)
{
    if (prime_cnt >= prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 64;
        uint64_t *tmp = (uint64_t *)realloc(primes_u64, new_cap * sizeof(uint64_t));
        primes_u64 = tmp;
        prime_cap  = new_cap;
    }
}

static void ensure_qual_capacity(void)
{
    if (qual_cnt >= qual_cap) {
        size_t new_cap = qual_cap ? qual_cap * 2 : 64;
        uint64_t *tmp = (uint64_t *)realloc(qual_u64, new_cap * sizeof(uint64_t));
        qual_u64 = tmp;
        qual_cap = new_cap;
    }
}

/* trial division using primes already found (all primes <= candidate-1) */
static bool is_prime_uint64(uint64_t p)
{
    if (p < 2) return false;
    if (p == 2 || p == 3) return true;
    if ((p & 1) == 0) return false;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t pr = primes_u64[i];
        if (pr * pr > p) break;
        if (p % pr == 0) return false;
    }
    return true;
}

static void add_prime(uint64_t p)
{
    ensure_prime_capacity();
    primes_u64[prime_cnt++] = p;
}

static void add_qual_prime(uint64_t p)
{
    ensure_qual_capacity();
    qual_u64[qual_cnt++] = p;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1, but guard against 0 */

    if (!initialized) {
        /* initialise prime list with 2   */
        ensure_prime_capacity();
        primes_u64[prime_cnt++] = 2;
        initialized = true;
        /* we start testing from 3 (the first odd > 2) */
        candidate = 3;
    }

    /* generate qualifying primes until we reach the n-th one */
    while (qual_cnt < n) {
        uint64_t p = candidate;
        if (is_prime_uint64(p)) {
            add_prime(p);
            if (p % 9 == 1 || p % 9 == 4) {
                add_qual_prime(p);
            }
        }
        candidate += 2;   /* skip even numbers to improve speed */
    }

    return (seq_t)qual_u64[n - 1];
}
