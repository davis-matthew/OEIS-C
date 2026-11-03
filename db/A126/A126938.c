/*
 * A126938  a(1) = 3, a(n) = the smallest prime p > a(n-1) such that
 *          (a(n-1)+p)/2 is prime.
 * https://oeis.org/A126938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 3; for n > 1, a(n) is the smallest prime p > a(n‑1) such
 *            that (a(n‑1)+p)/2 is also prime
 * Range:     u8 n <= 10, u16 n <= 404, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 63 terms through n = 940 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;      /* current capacity of cache     */
    static idx_t  computed = 0;   /* number of terms already filled  */

    if (n == 0)
        return (seq_t)0;          /* undefined for n=0, but harmless */

    /* Return a previously computed term if possible */
    if (n <= computed)
        return cache[n - 1];

    /* Allocate or expand cache to at least one element */
    if (cap < 1) {
        cache = mem_resize(seq_t, cache, 1);
        if (!cache) return (seq_t)0;     /* allocation failure */
        cap = 1;
    }

    /* Store the first term if not already stored */
    if (computed == 0) {
        cache[0] = (seq_t)3;   /* a(1) = 3 */
        computed = 1;
    }

    /* Ensure capacity for the required number of terms */
    if (n > cap) {
        size_t new_cap = cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) return (seq_t)0;   /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute terms from computed+1 up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t prev = cache[i - 2];          /* a(i-1) */

        /* Start searching for the next prime bigger than prev */
        seq_t cand = prev + (seq_t)1;
        if ((cand & (seq_t)1) == (seq_t)0)  /* make cand odd if even */
            cand += (seq_t)1;

        /* Find the smallest prime cand such that (prev + cand)/2 is prime */
        for (;;) {
            if (!is_prime_big(cand))
                cand += (seq_t)2;
            else {
                seq_t mid = (prev + cand) / (seq_t)2;
                if (is_prime_big(mid)) {
                    cache[i - 1] = cand;   /* a(i) = cand */
                    break;
                }
                cand += (seq_t)2;
            }
        }
    }

    computed = n;
    return cache[n - 1];
}
