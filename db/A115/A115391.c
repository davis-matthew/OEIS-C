/*
 * A115391  a(0)=0; then a(4*k+1)=a(4*k)+(4*k+1)^2,
 *          a(4*k+2)=a(4*k+1)+(4*k+3)^2, a(4*k+3)=a(4*k+2)+(4*k+2)^2,
 *          a(4*k+4)=a(4*k+3)+(4*k+4)^2.
 * https://oeis.org/A115391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 57, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   idx_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   a(0) = 0
   For n ≥ 1:
     let r = n & 3
     if r == 1: a(n) = a(n-1) + n²
     if r == 2: a(n) = a(n-1) + (n+1)²
     if r == 3: a(n) = a(n-1) + (n-1)²
     if r == 0: a(n) = a(n-1) + n²
*/
#include <stdlib.h>
#include <stdint.h>
#include "../utils/idx_to_seq.h"

/* Declare function prototype (required by external build ) */
seq_t a(idx_t n);

/* Helper: convert idx_t to seq_t safely */

seq_t a(idx_t n)
{
    /* static cache – grows as needed */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;   /* number of elements stored (>=1) */
    static idx_t  cache_cap = 0;   /* current allocation capacity */

    /* initialise cache for node 0 */
    if (cache_len == 0) {
        cache_cap = 1;
#if defined(__cplusplus)
        cache = new seq_t[cache_cap];
#else
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
#endif
        if (!cache) return 0;           /* out‑of‑memory fallback */
        cache[0] = 0;
        cache_len = 1;
    }

    /* grow cache if necessary */
    if (n + 1 > cache_cap) {
#if defined(__cplusplus)
        idx_t  new_cap = (idx_t)(cache_cap * 2);
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = new seq_t[new_cap];
        for (idx_t i = 0; i < cache_len; ++i) tmp[i] = cache[i];
        delete[] cache;
        cache = tmp;
#else
        idx_t  new_cap = cache_cap * 2;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* out‑of‑memory fallback */
        cache = tmp;
#endif
        cache_cap = new_cap;
    }

    /* compute values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t inc;
        switch (i & 3) {
            case 1: inc = idx_to_seq(i) * idx_to_seq(i);            break;
            case 2: inc = idx_to_seq(i + 1) * idx_to_seq(i + 1);    break;
            case 3: inc = idx_to_seq(i - 1) * idx_to_seq(i - 1);    break;
            case 0: inc = idx_to_seq(i) * idx_to_seq(i);            break;
        }
        cache[i] = cache[i - 1] + inc;
    }
    cache_len = n + 1;
    return cache[(size_t)n];
}
