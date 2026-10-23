/*
 * A046166  Number of minimal covers on n objects with 5 members.
 * https://oeis.org/A046166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   mul_small, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A046166: Number of minimal covers on n objects with 5 members
   Formula: a(n) = (31^n - 5*30^n + 10*29^n - 10*28^n + 5*27^n - 26^n) / 120
   Recurrence: a(n) = 171*a(n-1) - 12175*a(n-2) + 461985*a(n-3)
               - 9853624*a(n-4) + 112007964*a(n-5) - 530122320*a(n-6)
   Initial values: a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=1, a(6)=171
*/

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"
#include "../utils/mul_small.h"

/* cache for integer types (≤ 8 bytes) */
static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* last index computed and stored in cache */
static idx_t cache_cap = 0;   /* cache array capacity */

static void ensure_int_cache(idx_t n)
{
    /* enlarge cache array if necessary */
    if (n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;   /* allocate indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* unrecoverable */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute initial values a(1)..a(6) if not already done */
    idx_t end = n < 6 ? n : 6;
    for (idx_t i = cache_len + 1; i <= end; ++i) {
        seq_t p26 = pow_uint(26, i);
        seq_t p27 = pow_uint(27, i);
        seq_t p28 = pow_uint(28, i);
        seq_t p29 = pow_uint(29, i);
        seq_t p30 = pow_uint(30, i);
        seq_t p31 = pow_uint(31, i);

        seq_t val = p31;
        val -= mul_small(p30, 5);
        val += mul_small(p29, 10);
        val -= mul_small(p28, 10);
        val += mul_small(p27, 5);
        val -= p26;
        /* division by 120 (5!) is exact for these small exponents */
        cache[i] = val / 120;
    }
    if (cache_len < end) cache_len = end;

    /* extend the cache using the 6‑term recurrence */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        __uint128_t t = 0;
        t += (__uint128_t)171      * (__uint128_t)cache[k - 1];
        t -= (__uint128_t)12175    * (__uint128_t)cache[k - 2];
        t += (__uint128_t)461985   * (__uint128_t)cache[k - 3];
        t -= (__uint128_t)9853624  * (__uint128_t)cache[k - 4];
        t += (__uint128_t)112007964* (__uint128_t)cache[k - 5];
        t -= (__uint128_t)530122320* (__uint128_t)cache[k - 6];
        cache[k] = (seq_t) t;  /* cast truncates for 64‑bit build */
    }
    cache_len = n;
}

/* compute the term directly for arbitrary‑precision types */
static seq_t compute_big(idx_t n)
{
    seq_t p26 = pow_uint(26, n);
    seq_t p27 = pow_uint(27, n);
    seq_t p28 = pow_uint(28, n);
    seq_t p29 = pow_uint(29, n);
    seq_t p30 = pow_uint(30, n);
    seq_t p31 = pow_uint(31, n);

    seq_t val = p31;
    val -= mul_small(p30, 5);
    val += mul_small(p29, 10);
    val -= mul_small(p28, 10);
    val += mul_small(p27, 5);
    val -= p26;
    return val / 120;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* below OEIS offset, arbitrary */

    if (sizeof(seq_t) <= 8) {              /* integer types (≤ 64‑bit) */
        ensure_int_cache(n);
        return cache[n];
    } else {                                /* arbitrary–precision type */
        return compute_big(n);
    }
}
