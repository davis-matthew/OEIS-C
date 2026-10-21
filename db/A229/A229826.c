/*
 * A229826  Evil (A001969) numbers divisible by 7 but not divisible by 3.
 * https://oeis.org/A229826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Evil numbers divisible by 7 but not by 3
   a(n) = the n-th integer of the form 7*k, 
   where k > 0, k mod 3 ≠ 0, and popcount(7*k) is even.
   Offset: 1
   Any programming language that implements arithmetic, bitwise
   operations, and dynamic allocation can use this code.   */

#include <stdbool.h>
#include <stdlib.h>

static seq_t *cache = NULL;       /* cache of generated terms   */
static size_t cache_size = 0;     /* number of terms currently cached   */
static size_t cache_cap  = 0;     /* allocated capacity of the cache    */
static idx_t last_k = 0;          /* last k value processed (k in 7*k) */

/* Ad hoc popcount parity checker: returns true if x has an even
   number of 1‑bits.  Works for all seq_t types supporting '&' and '-'. */
static bool is_even_popcount(seq_t x)
{
    unsigned int parity = 0;          /* 0 = even, 1 = odd */
    while (x != 0) {
        parity ^= 1;
        x &= x - 1;                   /* clear lowest set bit */
    }
    return parity == 0;
}

seq_t a(idx_t n)
{
    /* Return cached value if already computed */
    if (n > 0 && (size_t)n <= cache_size)
        return cache[(size_t)n - 1];

    /* Allocate increasing capacity when needed */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < (size_t)n)
            new_cap *= 2;

#ifdef __cplusplus
        seq_t *new_cache = reinterpret_cast<seq_t *>(
            realloc(cache, new_cap * sizeof(seq_t)));
#else
        seq_t *new_cache = realloc(cache, new_cap * sizeof(seq_t));
#endif
        if (!new_cache)
            abort();  /* allocation failed */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Generate terms until we reach term n */
    for (idx_t k = last_k + 1; ; ++k) {
        if (k % 3 == 0)                     /* skip multiples of 3 */
            continue;

        seq_t val = (seq_t)7 * k;           /* divisible by 7          */
        if (is_even_popcount(val)) {        /* evil number            */
            cache[cache_size++] = val;
            last_k = k;
            if (cache_size == (size_t)n)
                break;
        }
    }
    return cache[(size_t)n - 1];
}
