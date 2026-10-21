/*
 * A061595  Product of digits + 1 is prime, sum of digits + 1 is prime and sum
 *          of digits - 1 is prime.
 * https://oeis.org/A061595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1003, u32 n <= 1003, u64 n <= 1003,
 *            u128 n <= 1003, big any n
 * Helpers:   digit_product, digit_sum, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Properties: digits of x satisfy
 *   product(x) + 1  is prime
 *   sum(x)   + 1  is prime
 *   sum(x)   - 1  is prime
 * a(n) is the n‑th positive integer with the above property
 * (a(1) = 4). The sequence is generated on demand by scanning
 * integers in increasing order.  The implementation is
 * memoised; arbitrary order of calls is supported.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"
#include "../utils/digit_product.h"
#include "../utils/is_prime_small.h"

static seq_t *cache = NULL;            /* stored terms */
static size_t cache_cap = 0;           /* allocated capacity   */
static idx_t cache_len = 0;            /* number of stored terms */
static seq_t next_candidate = 1;       /* next integer to test */

/* ensure the cache can hold at least "need" elements */
static void ensure_cache_cap(size_t need) {
    if (cache_cap < need) {
        size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                 /* unlikely in practice */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* check whether x meets all three primality conditions */
static bool good_number(seq_t x) {
    seq_t sum = digit_sum(x);
    if (sum < 2) return false;              /* ensures sum‑1 >= 1 */
    seq_t prod = digit_product(x);
    return is_prime_small(prod + (seq_t)1) &&
           is_prime_small(sum + (seq_t)1) &&
           is_prime_small(sum - (seq_t)1);
}

/* sequence accessor */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;            /* safety for offset < 1 */

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        if (good_number(next_candidate)) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
