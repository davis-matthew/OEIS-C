/*
 * A032664  Digit '1' concatenated with a(n) is a prime.
 * https://oeis.org/A032664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1178, u32 n <= 57, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, is_prime_big, pow10, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate a(n) by scanning natural numbers m and testing if
 * 1 concatenated in front of m = "1"||m is a prime.  Results are memoized
 * in a static array so that repeated and out-of-order queries are fast. */

#include <stdbool.h>
#include "../utils/pow10.h"         /* seq_t pow10(idx_t e); */
#include "../utils/digit_len.h"     /* int digit_len(seq_t x); */
#include "../utils/is_prime_big.h"  /* bool is_prime_big(seq_t n); */
#include "../utils/seq_alloc.h"     /* seq_t* seq_alloc(size_t cap); */
#include "../utils/seq_realloc.h"   /* seq_t* seq_realloc(seq_t* old, size_t oldcap, size_t newcap); */

static seq_t *cache = NULL;      /* stores a(1), a(2), ... */
static size_t cache_len = 0;     /* number of cached terms      */
static size_t cache_cap = 0;     /* current capacity of cache    */
static idx_t cur_m = 1;          /* next integer to test for a(n) */

static void grow_cache_if_needed(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        if (cache == NULL)
            cache = seq_alloc(new_cap);
        else
            cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }
}

static void ensure_n_terms(idx_t n)
{
    if (n == 0) return;
    if (cache_len >= n) return;    /* already have enough terms */

    while (cache_len < n) {
        seq_t val = (seq_t)cur_m;            /* current candidate    */
        int d   = digit_len(val);          /* number of digits in m */
        seq_t concat = pow10((idx_t)d) + val; /* 1 followed by m      */

        if (is_prime_big(concat)) {
            grow_cache_if_needed();
            cache[cache_len++] = val;     /* store a(n)            */
        }
        ++cur_m;                          /* advance to next integer  */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not defined for n<1 */
    ensure_n_terms(n);
    return cache[n-1];
}
