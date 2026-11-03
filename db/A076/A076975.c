/*
 * A076975  a(1) = 1, a(2n) = smallest composite number > (2n-1)-th partial
 *          sum of the sequence itself and a(2n+1) = smallest prime > 2n-th
 *          partial sum of the sequence.
 * https://oeis.org/A076975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; For n > 1: if n is even: a(n) = smallest composite >
 *            sum_{i=1}^{n-1} a(i) if n is odd : a(n) = smallest prime >
 *            sum_{i=1}^{n-1} a(i)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 32 (2026-11-03)
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

/* Cached sequence values and partial sum.
   The cache grows dynamically on first access. */
static seq_t *cache = NULL;           /* array of computed a(i) */
static idx_t  cache_count = 0;        /* number of terms currently cached   */
static seq_t  partial_sum = 0;        /* sum of first cache_count terms     */

/* Return the smallest prime > start (start is a seq_t). */
static inline seq_t next_prime_from(seq_t start)
{
    seq_t v = start + (seq_t)1;
    while (!is_prime_big(v))
        ++v;
    return v;
}

/* Return the smallest composite > start. */
static inline seq_t next_composite_from(seq_t start)
{
    seq_t v = start + (seq_t)1;
    while (v <= (seq_t)1 || is_prime_big(v))
        ++v;
    return v;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined, return 0 for safety */
        return (seq_t)0;

    /* compute terms up to n if necessary */
    if (n > cache_count) {
        size_t newsize = (size_t)n;
        cache = mem_resize(seq_t, cache, newsize);
        /* extend cache (partial_sum already holds sum up to cache_count) */
        for (idx_t i = cache_count + (idx_t)1; i <= n; ++i) {
            seq_t term;
            if (i == (idx_t)1) {
                term = (seq_t)1;
            } else if ((i & (idx_t)1) == (idx_t)0) {   /* even index */
                term = next_composite_from(partial_sum);
            } else {                                 /* odd index */
                term = next_prime_from(partial_sum);
            }
            cache[i - (idx_t)1] = term;
            partial_sum += term;
        }
        cache_count = n;
    }
    return cache[n - (idx_t)1];
}
