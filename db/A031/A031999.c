/*
 * A031999  Numbers whose base-4 digits are in nonincreasing order.
 * https://oeis.org/A031999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 486, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate numbers whose base‑4 digits are in non‑increasing order
   by enumerating all digit sequences of increasing length.  The sequence
   is indexed starting at 1 (a(1)=1).  The implementation memoises all
   generated values in a dynamic array so that repeated calls are
   efficient and work for arbitrary order of n. */

#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/* Internal state */ 
static seq_t *cache = NULL;      /* dynamic array of terms */
static idx_t  cache_size = 0;    /* allocated capacity of cache   */
static idx_t  produced = 0;      /* number of terms stored so far */
static idx_t  last_len  = 0;     /* largest digit‑length generated */

/* Resize the cache to accommodate 'needed' elements. */
static void grow_cache(idx_t needed)
{
    if (needed <= cache_size)
        return;                      /* already large enough */
    idx_t new_cap = cache_size ? cache_size << 1 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_size = new_cap;
}

/* Recursive construction of all base‑4 numbers of a given length.
   'pos'   : current digit position (0 = most significant).
   'len'   : total number of digits desired.
   'prev'  : maximum allowed digit at this position so that digits are
             non‑increasing (d <= prev).
   'cur'   : number built so far (in base‑10). */
static void gen_rec(idx_t pos, idx_t len, unsigned int prev, seq_t cur)
{
    if (pos == len) {                     /* finished a number */
        if (produced == cache_size)
            grow_cache(cache_size ? cache_size << 1 : 16);
        cache[produced++] = cur;
        return;
    }
    unsigned int start = (pos == 0) ? 1 : 0;   /* most significant digit cannot be 0 */
    for (unsigned int d = start; d <= prev; ++d) {
        gen_rec(pos + 1, len, d, cur * 4 + d);
    }
}

/* Ensure that the cache contains at least 'n' terms.  If not, generate
   all numbers of increasing digit length until we have enough. */
static void generate_up_to(idx_t n)
{
    while (produced < n) {
        ++last_len;                       /* next digit length to generate */
        gen_rec(0, last_len, 3, 0);
    }
}

/* ------------------------------------------------------------ */
/* Public function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                         /* offset 1: a(1) is the first term */

    generate_up_to(n);                     /* produce terms up to n if needed */
    return cache[n - 1];                   /* zero‑based indexing in cache */
}
