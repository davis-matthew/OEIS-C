/*
 * A081878  Triangle read by rows in which row n begins with n and iterates
 *          the process of dividing n by 2 if n is even, adding 3 if n is an
 *          odd prime, otherwise adding 1; stopping when either 1 or 3 is
 *          reached.
 * https://oeis.org/A081878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes the triangle read by rows of A081878.
   Row n starts with n and iterates:
     even  → n/2
     odd prime → n+3
     odd composite → n+1
   Sequence stops when a value 1 or 3 is reached.
   a(1)=1, a(2)=2, a(3)=1, a(4)=3, a(5)=4, a(6)=2, …
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

/* static cache of computed sequence terms */
static seq_t *cache = NULL;        /* array storing a(k) for k=1..cache_len */
static idx_t  cache_len = 0;      /* number of terms currently stored */
static idx_t  cache_cap = 0;      /* allocated capacity of cache array */

/* bookkeeping for lazy generation */
static idx_t  current_row      = 1;   /* next row number to generate */
static idx_t  cumulative_terms = 0;   /* total terms generated so far */

/* small prime test for 64‑bit numbers */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)           return false;
    if (n == 2)          return true;
    if ((n & 1ULL) == 0) return false;
    for (uint64_t d = 3; d <= n / d; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* ensure cache has enough space, growing if necessary */
static void ensure_cache_capacity(idx_t required)
{
    if (required <= cache_cap) return;
    if (cache_cap == 0) cache_cap = 1024;
    while (cache_cap < required)
        cache_cap <<= 1;                /* multiply by 2 */
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
}

/* generate rows until we have at least `target` terms */
static void grow_until(idx_t target)
{
    while (cumulative_terms < target) {
        /* generate row `current_row` */
        seq_t x = (seq_t)current_row;
        for (;;) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = x;
            cumulative_terms++;

            if (x == (seq_t)1 || x == (seq_t)3)
                break;

            if ((x & (seq_t)1) == 0) {           /* even */
                x = x / (seq_t)2;
            } else {                              /* odd */
                uint64_t val = (uint64_t)x;       /* safe cast for small values */
                if (is_prime_uint64(val))
                    x = x + (seq_t)3;
                else
                    x = x + (seq_t)1;
            }
        }
        current_row++;
    }
}

seq_t a(idx_t n)         /* external linkage as required */
{
    if (n == 0) return (seq_t)0;       /* safety for offset 1 */
    grow_until(n);
    return cache[n - 1];
}
