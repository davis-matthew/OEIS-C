/*
 * A151867  a(0)=0; a(1)=0; a(2)=0; for n>=3 if n=2^i + j with 0<=j<2^i then
 *          a(n)=2*a(j) + 2*a(j + 1) except we add 1 if j=2^i-1.
 * https://oeis.org/A151867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   high_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is defined by:
   a(0)=a(1)=a(2)=0,
   for n>=3, write n = 2^i + j with 0 <= j < 2^i.
   Then a(n)=2*a(j)+2*a(j+1)   if j != 2^i-1,
   and a(n)=2*a(j)+2*a(j+1)+1 if j==2^i-1.
*/
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/high_pow2.h"

/* Cache for computed values */
static seq_t *cache = NULL;
static bool   *got   = NULL;
static idx_t   cache_size = 0;

static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;
    idx_t new_size = cache_size ? cache_size << 1 : 16;
    while (new_size <= n) new_size <<= 1;
    seq_t *tmp_seq = NULL;
    bool  *tmp_bool = NULL;
#if defined(__cplusplus)
    tmp_seq  = reinterpret_cast<seq_t*>(
                   realloc(static_cast<void*>(cache), new_size * sizeof(seq_t)));
    tmp_bool = reinterpret_cast<bool*>(
                   realloc(static_cast<void*>(got),   new_size * sizeof(bool)));
#else
    tmp_seq  = realloc(cache, new_size * sizeof(seq_t));
    tmp_bool = realloc(got,   new_size * sizeof(bool));
#endif
    if (!tmp_seq || !tmp_bool) exit(1);    /* allocation failed */
    for (idx_t i = cache_size; i < new_size; ++i) {
        tmp_seq[i]  = 0;
        tmp_bool[i] = false;
    }
    cache = tmp_seq;
    got   = tmp_bool;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;
    ensure_cache(n);
    if (got[n]) return cache[n];

    idx_t p = high_pow2(n);          /* 2^i */
    idx_t j = n - p;                 /* j < 2^i */

    seq_t val = a(j);
    val *= 2;                        /* 2*a(j) */
    seq_t t = a(j + 1);
    val += t * 2;                    /* + 2*a(j+1) */
    if (j == p - 1) val += 1;        /* add one when j = 2^i-1 */

    cache[n] = val;
    got[n]   = true;
    return val;
}
