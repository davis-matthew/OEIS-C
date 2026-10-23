/*
 * A041140  Numerators of continued fraction convergents to sqrt(79).
 * https://oeis.org/A041140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 33, u128 n <= 69,
 *            big any n
 * Helpers:   get_coeff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A041140 – numerators of continued‑fraction convergents to sqrt(79)
   Recurrence:
      a(0) = 8
      a(1) = 9
      For n ≥ 2:
      a(n) = c(n) * a(n-1) + a(n-2)
   where the coefficient c(n) repeats with period 4 starting from n = 2:
      c(2) = 7, c(3) = 1, c(4) = 16, c(5) = 1, and so on.
   This follows from the continued‑fraction expansion
      sqrt(79) = [8; 1, 7, 1, 16, 1, 7, 1, 16, …].
*/

#include <stdlib.h>
#include "../utils/get_coeff.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of stored terms */
static size_t cache_cap = 0;   /* allocated capacity */

/* Ensure capacity for at least 'need' elements */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* In the unlikely event of allocation failure, the program will abort,
       which is acceptable for the build tools that call this function. */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Initialize cache lazily */
    if (cache_len == 0) {
        ensure_capacity(16);
        cache[0] = (seq_t)8;   /* a(0) */
        cache_len = 1;
    }

    if (n < cache_len)
        return cache[n];

    /* If cache contains only a(0), compute a(1) immediately */
    if (cache_len == 1) {
        cache[1] = (seq_t)1 * cache[0] + (seq_t)1;  /* a(1) = 1*8 + 1 */
        cache_len = 2;
    }

    /* Compute missing terms up to n */
    while (cache_len <= n) {
        idx_t i = (idx_t)cache_len;
        seq_t coeff = (seq_t)get_coeff(i);
        seq_t new_val = coeff * cache[cache_len - 1] + cache[cache_len - 2];
        ensure_capacity(cache_len + 1);
        cache[cache_len] = new_val;
        ++cache_len;
    }

    return cache[n];
}
