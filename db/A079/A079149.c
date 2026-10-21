/*
 * A079149  Primes p such that either p-1 or p+1 has at most 2 prime factors,
 *          counted with multiplicity; i.e., primes p such that either
 *          bigomega(p-1) <= 2 or bigomega(p+1) <= 2, where bigomega(n) =
 *          A001222(n).
 * https://oeis.org/A079149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A079149:
   primes p such that bigomega(p‑1) ≤ 2 or bigomega(p+1) ≤ 2.
   a(1) = 2, a(2) = 3, a(3) = 5, ...  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bigomega.h"

/* ---------- static data ---------- */
static uint32_t *prime_list = NULL;      /* primes found so far   */
static size_t   prime_cnt  = 0;          /* number of primes      */
static size_t   prime_cap  = 0;          /* allocated capacity    */

static seq_t   *qual_list = NULL;       /* qualifying primes    */
static size_t  qual_cnt  = 0;            /* number of qualifying */
static size_t  qual_cap  = 0;            /* allocated capacity    */

static uint64_t next_candidate = 3;     /* next odd number to test */
static bool     initialized = false;

/* ---------- helpers for prime list ---------- */
static void grow_prime_list(void)
{
    if (prime_cnt >= prime_cap) {
        size_t new_cap = prime_cap ? (prime_cap << 1) : 64;
        uint32_t *tmp = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
        if (!tmp) abort();
        prime_list = tmp;
        prime_cap  = new_cap;
    }
}

static void add_prime(uint32_t p)
{
    grow_prime_list();
    prime_list[prime_cnt++] = p;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * (uint64_t)p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* ---------- helpers for qualifying list ---------- */
static void grow_qual_list(void)
{
    if (qual_cnt >= qual_cap) {
        size_t new_cap = qual_cap ? (qual_cap << 1) : 64;
        seq_t *tmp = (seq_t *)realloc(qual_list, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        qual_list = tmp;
        qual_cap  = new_cap;
    }
}

static void add_qual(uint64_t p)
{
    grow_qual_list();
    qual_list[qual_cnt++] = (seq_t)p;   /* implicit conversion */
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* initialise on first call */
    if (!initialized) {
        /* prime list starts with 2 */
        prime_cap   = 64;
        prime_list  = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
        if (!prime_list) abort();
        prime_list[prime_cnt++] = 2;

        /* qualifying list starts with 2 */
        qual_cap   = 64;
        qual_list  = (seq_t *)malloc(qual_cap * sizeof(seq_t));
        if (!qual_list) abort();
        qual_list[qual_cnt++] = (seq_t)2;   /* a(1) = 2 */

        next_candidate = 3;
        initialized = true;
    }

    /* generate qualifying primes until we have n terms */
    while (qual_cnt < (size_t)n) {
        uint64_t p = next_candidate;
        next_candidate += 2;   /* only odd numbers */

        if (!is_prime_uint64(p))
            continue;

        /* add prime to list for future tests */
        add_prime((uint32_t)p);

        /* test bigomega(p-1) and bigomega(p+1) */
        idx_t omega1 = (idx_t)bigomega(p - 1);
        if (omega1 <= 2) {
            add_qual(p);
            continue;
        }
        idx_t omega2 = (idx_t)bigomega(p + 1);
        if (omega2 <= 2) {
            add_qual(p);
        }
    }

    return qual_list[n - 1];
}
