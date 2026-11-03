/*
 * A195104  Smallest prime p such that p*t(n) +- 1 is a twin prime pair, where
 *          t(n)=A014574(n) is the n-th twin prime average.
 * https://oeis.org/A195104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 40, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9795 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A195104
   a(n) = smallest prime p such that p * t(n) ± 1 are both prime,
   where t(n) is the nth twin prime average (A014574).
   The function generates twin averages on demand and then searches
   for the smallest prime p satisfying the condition.
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
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

/* --- cache for twin prime averages (A014574) -------------------------------- */

static seq_t *twin_cache = NULL;          /* array of averages          */
static size_t twin_cap   = 0;              /* allocated capacity         */
static idx_t  twin_cnt   = 0;              /* stored averages count      */
static idx_t  next_prime_candidate = 3;   /* next integer to test       */

/* Ensure twin_cache contains at least n entries (n >= 1). */
static void ensure_twin(idx_t n)
{
    if (twin_cap == 0) {
        twin_cap = 64;
        twin_cache = mem_new(seq_t, twin_cap);
        if (!twin_cache) abort();
    }

    while (twin_cnt < n) {
        /* Find next prime */
        while (!is_prime(next_prime_candidate))
            next_prime_candidate++;

        /* Check for twin prime */
        if (is_prime(next_prime_candidate + 2)) {
            if (twin_cap <= twin_cnt) {                /* grow cache */
                twin_cap *= 2;
                seq_t *tmp = mem_resize(seq_t, twin_cache, twin_cap);
                if (!tmp) abort();
                twin_cache = tmp;
            }
            twin_cache[twin_cnt++] = (seq_t)(next_prime_candidate + 1); /* average */
        }
        next_prime_candidate++;                         /* advance past this prime */
    }
}

/* --- main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1; undefined for n==0 */

    /* Ensure we have the required twin average */
    ensure_twin(n);
    seq_t t = twin_cache[n - 1];        /* t(n) = A014574(n) */

    /* Search for the smallest prime p such that p*t ± 1 are both prime */
    idx_t p = 2;
    for (;;) {
        if (is_prime(p)) {
            seq_t m = (seq_t)p * t;
            if (is_prime_big(m - 1) && is_prime_big(m + 1))
                return (seq_t)p;
        }
        p++;
    }
}
