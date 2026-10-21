/*
 * A090890  30*a(n) is the gap between sexy prime triples in the n-th sexy
 *          prime triple triple whose initial term is 11.
 * https://oeis.org/A090890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 3, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  a(n) is the n‑th value d such that  
 *  (11+30*d, 17+30*d, 23+30*d) and
 *  (11+60*d, 17+60*d, 23+60*d) are all prime.
 *  The first such d is 291, giving a(1)=291.
 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;     /* array of computed terms */
static size_t cache_len = 0;    /* number of terms stored */
static size_t cache_cap = 0;    /* current capacity of cache */
static idx_t next_d = 0;        /* last d that satisfied the condition */

static void ensure_cache_cap(void)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

static seq_t find_next_a(void)
{
    idx_t d = next_d + 1;
    const seq_t thirty = (seq_t)30;
    const seq_t sixty  = (seq_t)60;

    for (;;) {
        /* numbers for k = d */
        seq_t n1 = 11 + thirty * (seq_t)d;
        seq_t n2 = 17 + thirty * (seq_t)d;
        seq_t n3 = 23 + thirty * (seq_t)d;

        /* numbers for k = 2d */
        seq_t m1 = 11 + sixty  * (seq_t)d;
        seq_t m2 = 17 + sixty  * (seq_t)d;
        seq_t m3 = 23 + sixty  * (seq_t)d;

        if (is_prime_big(n1) && is_prime_big(n2) && is_prime_big(n3) &&
            is_prime_big(m1) && is_prime_big(m2) && is_prime_big(m3)) {
            next_d = d;
            return (seq_t)d;
        }
        ++d;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) abort();   /* sequence is 1‑based */

    while (cache_len < (size_t)n) {
        ensure_cache_cap();
        cache[cache_len] = find_next_a();
        ++cache_len;
    }
    return cache[n - 1];
}
