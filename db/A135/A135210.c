/*
 * A135210  Numbers n such that Sum_digits(n) + Sum_digits(n+1) =
 *          Sum_digits(2*n+1).
 * https://oeis.org/A135210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A135210:
   a(n) = the n‑th integer x such that
   sum_digits(x) + sum_digits(x+1) = sum_digits(2*x+1).
   Offset: 1 (a(1)=0). */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_sum.h"

#ifdef __cplusplus
#  include <vector>
static std::vector<seq_t> cache;          /* stored terms      */
static seq_t next_num = 0;                /* next integer to test */
#else
/* C implementation: dynamic array */
static seq_t *cache = NULL;               /* array of terms    */
static size_t cache_size = 0;             /* number of terms   */
static size_t cache_cap   = 0;             /*allocated capacity*/
static seq_t next_num = 0;                /* next integer to test */

static void grow_cache(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
        while (new_cap < need) new_cap <<= 1;
        seq_t *new_ptr = realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) abort();           /* memory error */
        cache = new_ptr;
        cache_cap = new_cap;
    }
}

static void add_term(seq_t v)
{
    grow_cache(cache_size + 1);
    cache[cache_size++] = v;
}
#endif

static void generate_next(void)
{
    seq_t v = next_num;
    if (digit_sum(v) + digit_sum(v + 1) == digit_sum(v * 2 + 1))
#ifdef __cplusplus
        cache.push_back(v);
#else
        add_term(v);
#endif
    next_num++;
}

static void ensure_cache(idx_t n)
{
    if (n == 0) return;           /* offset is 1; callers never ask for n=0 */
#ifdef __cplusplus
    while (cache.size() < n) generate_next();
#else
    while (cache_size < n) generate_next();
#endif
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    /* the sequence is 1‑based; cache element 0 corresponds to a(1) */
#ifdef __cplusplus
    return cache[n - 1];
#else
    return cache[n - 1];
#endif
}
