/*
 * A209843  Number of (n+1)X4 0..3 arrays with every 2X2 subblock having one
 *          or two distinct values, and new values 0..3 introduced in row
 *          major order.
 * https://oeis.org/A209843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 43*a(n-1) - 459*a(n-2) - 321*a(n-3) + 13084*a(n-4) -
 *            11850*a(n-5) - 84560*a(n-6) + 143072*a(n-7) + 6528*a(n-8) -
 *            98304*a(n-9) + 32768*a(n-10) Initial values (a(1) … a(10)): 227,
 *            5083, 115435, 2713195, 64857323, 1569132907, 38265187051,
 *            938086148459, 23077699792619, 569025498679659
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_realloc.h"

static const seq_t init_vals[10] = {
    (seq_t)227ULL,
    (seq_t)5083ULL,
    (seq_t)115435ULL,
    (seq_t)2713195ULL,
    (seq_t)64857323ULL,
    (seq_t)1569132907ULL,
    (seq_t)38265187051ULL,
    (seq_t)938086148459ULL,
    (seq_t)23077699792619ULL,
    (seq_t)569025498679659ULL
};

static seq_t *cache      = NULL;   /* array holding computed terms */
static idx_t  cache_cap  = 0;       /* current allocated capacity */
static idx_t  max_computed = 0;     /* highest index already computed */

static void ensure_cache(idx_t new_cap)
{
    if (new_cap <= cache_cap) return;
    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
    if (tmp) {
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset < 1 */

    if (n <= 10) return init_vals[n-1];   /* initial values */

    /* initialise first 10 terms once, if not already done */
    if (max_computed < 10) {
        ensure_cache(10);
        for (idx_t i = 0; i < 10; ++i)
            cache[i] = init_vals[i];
        max_computed = 10;
    }

    if (n <= max_computed) return cache[n-1];  /* value already computed */

    ensure_cache(n);
    for (idx_t i = max_computed; i < n; ++i) {
        seq_t term = 0;
        term += (seq_t)43      * cache[i-1];
        term -= (seq_t)459     * cache[i-2];
        term -= (seq_t)321     * cache[i-3];
        term += (seq_t)13084   * cache[i-4];
        term -= (seq_t)11850   * cache[i-5];
        term -= (seq_t)84560   * cache[i-6];
        term += (seq_t)143072  * cache[i-7];
        term += (seq_t)6528    * cache[i-8];
        term -= (seq_t)98304   * cache[i-9];
        term += (seq_t)32768   * cache[i-10];
        cache[i] = term;
    }

    max_computed = n;
    return cache[n-1];
}
