/*
 * A156611  a(1) = 2, a(n+1) is smallest prime >= a(n)+2*n.
 * https://oeis.org/A156611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2, a(n+1) = smallest prime ≥ a(n) + 2*n (n ≥ 1)
 * Range:     u8 n <= 15, u16 n <= 253, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9825 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequencing logic relies on a generic prime test suitable for any seq_t.
   Use the bundled is_prime_big routine to handle arbitrary‑precision types
   and small integral types alike. */
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
    /* 1‑indexed cache of computed terms */
    static seq_t *cache = NULL;      /* pointer to array of seq_t */
    static size_t cache_cap = 0;     /* allocated capacity in elements */
    static idx_t last_computed = 0;  /* highest n for which cache is valid */

    if (n == 0) return 0;                 /* undefined for n<1, harmless */

    /* Grow the cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < (size_t)n) new_cap *= 2;
        /* Explicit cast so that C++ accepts the void* result of realloc */
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i == 1) {                 /* a(1) = 2 */
            cache[0] = (seq_t)2;
            continue;
        }

        seq_t prev = cache[i - 2];                       /* a(i) = a(i-1) */
        seq_t inc  = (seq_t)((i - 1) * 2);               /* 2*(i‑1) */
        seq_t cand = prev + inc;                         /* lower bound */

        /* Skip even numbers > 2 */
        if (cand > (seq_t)2 && !(cand & (seq_t)1))
            cand += (seq_t)1;

        /* Find the smallest prime ≥ cand */
        while (!is_prime_big(cand))
            cand += (seq_t)2;     /* only odd increments after the first odd */

        cache[i - 1] = cand;      /* store a(i) */
    }

    last_computed = n;
    return cache[n - 1];
}
