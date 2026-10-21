/*
 * A225977  Number of n X 3 binary arrays whose sum with another n X 3 binary
 *          array containing no more than two 1s has rows and columns in
 *          lexicographically nondecreasing order.
 * https://oeis.org/A225977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 28, u64 n <= 101, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A225977: Number of n×3 binary arrays whose sum with another n×3 binary array
   containing at most two 1's has rows and columns in lexicographically
   nondecreasing order.
   Recurrence (n > 11):
       a(n) = 10*a(n-1) - 45*a(n-2) + 120*a(n-3) - 210*a(n-4)
              + 252*a(n-5) - 210*a(n-6) + 120*a(n-7)
              - 45*a(n-8) + 10*a(n-9) - a(n-10)
   Initial values:
       a(1)=8, a(2)=48, a(3)=252, a(4)=1178, a(5)=4722,
       a(6)=16361, a(7)=49811, a(8)=135672, a(9)=336189,
       a(10)=768900, a(11)=1642668. */

#include <stdlib.h>   /* malloc, realloc, abort */

static seq_t *cache = NULL;   /* cache[i] holds a(i+1) */
static idx_t cache_len = 0;   /* number of terms currently cached */

/* Ensure that cache contains at least the first n terms. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len)
        return;

    /* Reallocate cache to hold n terms. */
    seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
    if (!tmp) abort();           /* allocation failure */
    cache = tmp;

    /* Fill newly allocated part of the cache. */
    for (idx_t i = cache_len; i < n; ++i) {
        /* Initial values for i = 0 … 10 (a(1) … a(11)). */
        switch (i) {
            case 0:  cache[i] = (seq_t)8;           break;
            case 1:  cache[i] = (seq_t)48;          break;
            case 2:  cache[i] = (seq_t)252;         break;
            case 3:  cache[i] = (seq_t)1178;        break;
            case 4:  cache[i] = (seq_t)4722;        break;
            case 5:  cache[i] = (seq_t)16361;       break;
            case 6:  cache[i] = (seq_t)49811;       break;
            case 7:  cache[i] = (seq_t)135672;      break;
            case 8:  cache[i] = (seq_t)336189;      break;
            case 9:  cache[i] = (seq_t)768900;      break;
            case 10: cache[i] = (seq_t)1642668;     break;
            default:
                /* Recurrence for i ≥ 11 (i = n-1). */
                cache[i] =
                    (seq_t)10   * cache[i-1]
                  - (seq_t)45   * cache[i-2]
                  + (seq_t)120  * cache[i-3]
                  - (seq_t)210  * cache[i-4]
                  + (seq_t)252  * cache[i-5]
                  - (seq_t)210  * cache[i-6]
                  + (seq_t)120  * cache[i-7]
                  - (seq_t)45   * cache[i-8]
                  + (seq_t)10   * cache[i-9]
                  - cache[i-10];
                break;
        }
    }

    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n < 1, harmless */
    ensure_cache(n);              /* cache indices 0 .. n-1 */
    return cache[n-1];            /* a(1) is at cache[0] */
}
