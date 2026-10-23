/*
 * A125286  a(0)=1. a(n) = a(n-1) + (the largest term among
 *          {a(0),a(1),...a(n-1)} that has the same number of positive
 *          divisors as n).
 * https://oeis.org/A125286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Implements A125286:
   a(0) = 1
   For n > 0,  a(n) = a(n‑1) + M(n),
   where M(n) = the largest a(k) among 0 ≤ k < n
                that has the same number of positive divisors as n.
   If no such k exists, M(n) = 0.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>    /* for memcpy */

/*---------------------------------------------------------------*/
/* Count the number of positive divisors of a 64‑bit integer      */
/* (works for n up to 2^63‑1; n may be 0 or 1 as well).          */
/*---------------------------------------------------------------*/
static size_t divisor_count_u64(uint64_t n)
{
    if (n <= 1) return 1;   /* d(1) = 1, d(0) unused */
    size_t cnt = 1;
    for (uint64_t p = 2; p * p <= n; ++p)
    {
        if (n % p == 0)
        {
            unsigned e = 0;
            while (n % p == 0)
            {
                n /= p;
                ++e;
            }
            cnt *= (e + 1);
        }
    }
    if (n > 1) cnt *= 2;
    return cnt;
}

/*---------------------------------------------------------------*/
/* Memoisation storage                                     */
/*---------------------------------------------------------------*/
static seq_t *a_cache   = NULL;   /* array of a(0)..a(n)          */
static size_t a_cap     = 0;      /* allocated capacity            */
static idx_t  a_len     = 0;      /* number of terms stored        */

static seq_t *max_by_d  = NULL;   /* max a(k) for each number of divisors d  */
static size_t maxd_cap  = 0;      /* allocated capacity of max_by_d          */

/*---------------------------------------------------------------*/
/* Grow a_cache to at least n+1 elements                        */
/* Handles both C and C++ archives                              */
/*---------------------------------------------------------------*/
static void ensure_a_capacity(idx_t n)
{
    size_t required = (size_t)n + 1;
    if (required <= a_cap) return;

    size_t new_cap = a_cap ? a_cap << 1 : 16;
    if (new_cap < required) new_cap = required;

#ifdef __cplusplus
    seq_t *tmp = new seq_t[new_cap];
    if (a_cache) {
        memcpy(tmp, a_cache, a_len * sizeof(seq_t));
        delete[] a_cache;
    }
    a_cache = tmp;
#else
    seq_t *tmp = realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* out of memory */
    a_cache = tmp;
#endif
    a_cap = new_cap;
}

/*---------------------------------------------------------------*/
/* Grow max_by_d so that it can index d (0 <= d < maxd_cap).      */
/*---------------------------------------------------------------*/
static void ensure_map_capacity(size_t d)
{
    if (d < maxd_cap) return;
    size_t new_cap = maxd_cap ? maxd_cap << 1 : 16;
    while (new_cap <= d) new_cap <<= 1;

#ifdef __cplusplus
    seq_t *tmp = new seq_t[new_cap];
    if (max_by_d) {
        memcpy(tmp, max_by_d, maxd_cap * sizeof(seq_t));
        delete[] max_by_d;
    }
    else {
        memset(tmp, 0, new_cap * sizeof(seq_t));
    }
    max_by_d = tmp;
#else
    seq_t *tmp = realloc(max_by_d, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    /* initialise new slots to zero */
    for (size_t i = maxd_cap; i < new_cap; ++i) tmp[i] = 0;
    max_by_d = tmp;
#endif
    maxd_cap = new_cap;
}

/*---------------------------------------------------------------*/
/* Build a(0)..a(n)                                           */
/*---------------------------------------------------------------*/
static void build_to(idx_t n)
{
    if (a_len > n) return;          /* already built up to n */

    ensure_a_capacity(n);
    for (idx_t i = a_len; i <= n; ++i)
    {
        seq_t val;
        if (i == 0)
        {
            val = (seq_t)1;
            a_cache[0] = val;

            size_t d_val = divisor_count_u64((uint64_t)val);
            ensure_map_capacity(d_val);
            if (val > max_by_d[d_val]) max_by_d[d_val] = val;
        }
        else
        {
            /* number of divisors of the index i */
            size_t d_i = divisor_count_u64((uint64_t)i);

            seq_t add = 0;
            if (d_i < maxd_cap) add = max_by_d[d_i];

            val = a_cache[i-1] + add;
            a_cache[i] = val;

            size_t d_val = divisor_count_u64((uint64_t)val);
            ensure_map_capacity(d_val);
            if (val > max_by_d[d_val]) max_by_d[d_val] = val;
        }
    }
    a_len = n + 1;
}

/*---------------------------------------------------------------*/
/* Public function                                              */
/*---------------------------------------------------------------*/
seq_t a(idx_t n)
{
    build_to(n);
    return a_cache[n];
}
