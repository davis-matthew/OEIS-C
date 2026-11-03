/*
 * A156672  a(1)=2, a(n+1) is the smallest prime >= a(n) + sum of digits of
 *          a(n).
 * https://oeis.org/A156672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(1)=2 and for n≥1, a(n+1) = the smallest prime ≥
 *            a(n)+sum_of_digits(a(n))
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 994 (2026-11-03)
 * Helpers:   digit_sum, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_sum.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Helper: allocate an array of seq_t and zero it. */
static seq_t *alloc_seq(size_t n)
{
    return mem_new(seq_t, n);
}

/* Helper: free an array allocated by alloc_seq. */
static void free_seq(seq_t *p)
{
    mem_free(p);
}

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

/* Compute the next term given the previous term */
static seq_t next_term(seq_t prev) {
    seq_t target = prev + digit_sum(prev);   /* a(n) + sum of digits of a(n) */
    seq_t p = target;
    while (!is_prime_big(p))                 /* find smallest prime ≥ target */
        ++p;
    return p;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;                    /* offset starts at 1, undefined for 0 */

    /* Static memoization */
    static seq_t *cache = NULL;
    static idx_t max_n = 0;
    static size_t cache_cap = 0;

    /* Expand cache if necessary */
    if (cache == NULL || cache_cap < (size_t)(n + 1)) {
        seq_t *new_cache = alloc_seq((size_t)(n + 1));
        if (cache != NULL) {
            /* copy existing values */
            for (idx_t i = 1; i <= max_n; ++i)
                new_cache[i] = cache[i];
            free_seq(cache);
        }
        cache = new_cache;
        cache_cap = (size_t)(n + 1);
    }

    /* Generate missing terms */
    if (n > max_n) {
        if (max_n < 1) {
            cache[1] = 2;                   /* a(1)=2 */
            max_n = 1;
        }
        for (idx_t i = max_n + 1; i <= n; ++i)
            cache[i] = next_term(cache[i - 1]);
        max_n = n;
    }

    return cache[n];
}
