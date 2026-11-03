/*
 * A216285  Primes which cannot be written as x^4+y^4.
 * https://oeis.org/A216285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 6517, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9454 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A216285 – primes that cannot be written as x^4 + y^4.
   The function keeps a cache of found terms and extends it
   as necessary.  It works for seq_t = unsigned integer
   (8/16/32/64/128 bits) or an arbitrary‑precision class. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
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

static seq_t *terms    = NULL;
static size_t  terms_sz = 0;   /* number of stored terms       */
static size_t  terms_cap = 0;  /* allocated capacity           */
static uint64_t last_checked = 1;  /* last integer examined (>=1) */

/* integer fourth‑root for 64‑bit values, result fits in uint64_t */
static uint64_t fourth_root(uint64_t x)
{
    uint64_t lo = 0, hi = 65536ULL;           /* 65536^4 == 2^64 */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        __uint128_t m4 = (__uint128_t)mid * mid * mid * mid;
        if (m4 <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* test whether a 64‑bit prime can be expressed as a^4 + b^4 (a,b >=1) */
static bool representable(uint64_t p)
{
    uint64_t a_max = fourth_root(p);
    for (uint64_t a = 1; a <= a_max; ++a) {
        __uint128_t a4 = (__uint128_t)a * a * a * a;
        if (a4 > p) break;
        uint64_t rem = p - (uint64_t)a4;
        uint64_t b = fourth_root(rem);
        if ((__uint128_t)b * b * b * b == rem)
            return true;          /* found a representation */
    }
    return false;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* undefined for offset 1, but guard */

    /* extend cache up to n terms */
    while (terms_sz < (size_t)n) {
        /* next candidate: odd integers starting from 3 */
        uint64_t cand = last_checked < 3 ? 3 : last_checked + 1;
        if (cand < 3) cand = 3;
        if ((cand & 1) == 0) cand++;   /* make it odd */

        /* test primality */
        if (is_prime_big((seq_t)cand)) {
            /* check representability as sum of two fourth powers */
            if (!representable((uint64_t)cand)) {
                /* store the term */
                if (terms_sz == terms_cap)
                    grow_seq(&terms, &terms_cap, terms_sz + 1);
                terms[terms_sz++] = (seq_t)cand;
            }
        }
        last_checked = cand;
    }

    return terms[(size_t)n - 1];
}
