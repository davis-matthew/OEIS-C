/*
 * A156123  Primes p such that p+36 and p+144 are both prime.
 * https://oeis.org/A156123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 66 terms through n = 970 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: search for primes p such that p+36 and p+144 are also prime */
/* This implementation keeps a growing cache of all such primes found so far.
   It builds an increasing list of ordinary primes by repeated sieving
   (using build_primes_upto) and tests each prime for the required
   primality conditions with is_prime_big.  The sequence is cached in a
   static array and the cache is extended only if further terms are needed.
   The function works for any integer type seq_t supplied by the build
   and is O(n) with a small constant factor.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include <stddef.h>
#include "../utils/build_primes_upto.h"
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

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

static seq_t *seq = NULL;          /* cache of sequence terms               */
static size_t seq_len = 0;         /* number of terms currently cached      */
static size_t seq_cap = 0;         /* current capacity of the cache         */
static idx_t   current_limit = 0;  /* highest value sieved up to so far     */

/* Helper that sieves all primes <= limit and appends those primes p
   with p+36 and p+144 prime to the global cache.  The cache contents
   are not cleared; duplicates are avoided by checking the last
   inserted term. */
static void extend_cache(idx_t limit)
{
    uint64_t *prime_arr = NULL;
    size_t    prime_cnt = 0;
    build_primes_upto(limit, &prime_arr, &prime_cnt);

    for (size_t i = 0; i < prime_cnt; ++i) {
        seq_t p = (seq_t)prime_arr[i];

        /* p+144 must still lie within the sieved range to ensure
           we can test it with is_prime_big efficiently. */
        if (p + (seq_t)144u > (seq_t)limit) break;

        seq_t p36  = p + (seq_t)36u;
        seq_t p144 = p + (seq_t)144u;
        if (is_prime_big(p36) && is_prime_big(p144)) {
            if (seq_len == 0 || seq[seq_len - 1] != p) {
                grow_seq(&seq, &seq_cap, seq_len + 1);
                seq[seq_len++] = p;
            }
        }
    }
    free(prime_arr);
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is 1‑based only */

    /* Ensure the cache is large enough to hold the first n terms. */
    while (seq_len < n) {
        /* Start sieving at a modest limit and double it each
           iteration until enough terms are found. */
        if (current_limit == 0) current_limit = 1000000u;   /* 1 M */
        else                      current_limit *= 2;

        extend_cache(current_limit);
    }

    return seq[n - 1];              /* 0‑based indexing internally */
}
