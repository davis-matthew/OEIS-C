/*
 * A065436  Duplicate of A055500.
 * https://oeis.org/A065436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) is the largest prime ≤ a(n‑1)+a(n‑2) with a(1)=a(2)=1
 * Range:     i8 n <= 12, i16 n <= 21, i32 n <= 21, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-03)
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

static seq_t *cache = NULL;         /* 1‑based array, 0 index unused */
static size_t cache_cap = 0;        /* allocated capacity (elements) */

/* Return a(n) for any n ≥ 1, computing values lazily and memoizing them. */
seq_t a(idx_t n)
{
    if (n == 0)                  /* undefined, but avoid UB */
        return (seq_t)0;

    /* Resize cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t old_cap = cache_cap;
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) {
            abort();            /* allocation failure */
        }
        cache = tmp;
        cache_cap = new_cap;
        /* zero‑initialize new entries */
        for (size_t i = old_cap; i < new_cap; ++i) {
            cache[i] = (seq_t)0;
        }
    }

    /* Ensure initial conditions */
    if (cache[1] == (seq_t)0) cache[1] = (seq_t)1;
    if (n >= 2 && cache[2] == (seq_t)0) cache[2] = (seq_t)1;

    /* Compute missing terms up to n */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t sum = cache[i - 1] + cache[i - 2];
        seq_t cand = sum;

        /* Make cand odd if it is > 2 and even */
        if (cand > (seq_t)2 && (cand & (seq_t)1) == (seq_t)0)
            cand -= (seq_t)1;

        while (!is_prime_big(cand) && cand > (seq_t)2) {
            cand -= (seq_t)2;   /* skip even numbers */
        }
        /* At this point either cand == 2 or cand is prime */
        cache[i] = cand;
    }

    return cache[n];
}
