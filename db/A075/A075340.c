/*
 * A075340  a(1) = 1; a(2n) is the smallest prime == 1 mod (a(2n-1)) and
 *          a(2n+1) is the smallest composite number == 1 (mod a(2n)).
 * https://oeis.org/A075340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 23, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A075340: a(1)=1; for n>=2,
   a(2k)   = smallest prime p such that p ≡ 1 (mod a(2k-1)),
   a(2k+1) = smallest composite c such that c ≡ 1 (mod a(2k)).
   This implementation caches results so calls can come in any order. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

/* static cache for computed terms */
static seq_t *cache = NULL;
static size_t cap = 0;
static size_t sz  = 0;

/* enlarge cache to hold at least 'need' elements */
static void ensure_capacity(size_t need)
{
    if (need <= cap) return;
    size_t newcap = cap ? cap << 1 : 16;
    while (newcap < need) newcap <<= 1;
    seq_t *newptr = mem_resize(seq_t, cache, newcap);
    if (!newptr) abort();           /* out of memory */
    cache = newptr;
    cap   = newcap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined below offset */
    /* compute terms up to n */
    if ((size_t)n > sz) {
        ensure_capacity((size_t)n);
        for (idx_t i = (idx_t)(sz + 1); i <= n; ++i) {
            size_t idx = (size_t)(i - 1);   /* 0‑based index */
            if (i == 1) {
                cache[idx] = (seq_t)1;
                continue;
            }
            seq_t prev = cache[idx - 1];
            seq_t cand = prev + (seq_t)1;    /* cand ≡ 1 (mod prev) */

            if ((i & 1ULL) == 0) {          /* even index -> prime */
                while (!is_prime_big(cand)) cand += prev;
            } else {                        /* odd index -> composite */
                while (is_prime_big(cand))  cand += prev;
            }
            cache[idx] = cand;
        }
        sz = (size_t)n;
    }
    return cache[(size_t)(n - 1)];
}
