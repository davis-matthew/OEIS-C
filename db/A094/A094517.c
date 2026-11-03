/*
 * A094517  Primes p such that 6p+11 is not a prime.
 * https://oeis.org/A094517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 975, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 976 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that 6p+11 is NOT a prime.
   a(1) = 11, a(2) = 19, a(3) = 29, ...   (OEIS A094517, offset 1) */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"
/* util for primality test on arbitrary-size integers */

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

/*--- dynamic array helpers -------------------------------------------*/
static void ensure_uint64_capacity(size_t needed, size_t *cap, uint64_t **buf)
{
    if (needed <= *cap) return;
    size_t newcap = (*cap == 0) ? 16 : *cap << 1;
    while (newcap < needed) newcap <<= 1;
    *buf = (uint64_t *)realloc(*buf, newcap * sizeof(uint64_t));
    *cap = newcap;
}

static void ensure_seq_capacity(size_t needed, size_t *cap, seq_t **buf)
{
    if (needed <= *cap) return;
    size_t newcap = (*cap == 0) ? 16 : *cap << 1;
    while (newcap < needed) newcap <<= 1;
    *buf = mem_resize(seq_t, *buf, newcap);
    *cap = newcap;
}

/*--- static state -----------------------------------------------------*/
static uint64_t cur = 2;                /* next integer to test for primality */
static uint64_t *prime_cache = NULL;    /* list of primes found so far */
static size_t prime_len = 0;            /* how many primes in prime_cache   */
static size_t prime_cap = 0;            /* capacity of prime_cache          */

static seq_t *desired = NULL;           /* list of special primes           */
static size_t desired_len = 0;          /* how many are stored              */
static size_t desired_cap = 0;          /* capacity of desired array        */

/*--- helper: test primality of 64‑bit integer using cached primes -------*/
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_len; ++i)
    {
        uint64_t p = prime_cache[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/*--- main sequence function -------------------------------------------*/
seq_t a(idx_t n)
{
    /* iterate until we have at least n special primes */
    while (desired_len < n)
    {
        /* test if cur is prime */
        if (is_prime_uint64(cur))
        {
            /* store it in the prime cache */
            ensure_uint64_capacity(prime_len + 1, &prime_cap, &prime_cache);
            prime_cache[prime_len++] = cur;

            /* compute 6*cur + 11 as seq_t and test composite */
            seq_t val = (seq_t)cur * (seq_t)6 + (seq_t)11;
            if (!is_prime_big(val))
            {
                /* store the special prime */
                ensure_seq_capacity(desired_len + 1, &desired_cap, &desired);
                desired[desired_len++] = (seq_t)cur;
            }
        }

        ++cur; /* move to next candidate */
    }

    /* return the (n‑1)‑indexed element (a(1) is first) */
    return desired[n - 1];
}
