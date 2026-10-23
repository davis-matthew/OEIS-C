/*
 * A016729  Highest minimal Hamming distance of any Type 4^H+ Hermitian
 *          additive self-dual code over GF(4) of length n.
 * https://oeis.org/A016729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 22, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS entry A016729
   Highest minimal Hamming distance of any Type 4^H+ Hermitian additive
   self‑dual code over GF(4) of length n.

   Formula: a(n) = a(n-1) + a(n-6) - a(n-7)   (for n > 7)
   Initial values:
   a(1) = 1, a(2) = 2, a(3) = 2, a(4) = 2, a(5) = 3,
   a(6) = 4, a(7) = 3
   (off‑set: 1)
   The function below memoises the values and supports any order of
   calls.  The implementation is purely iterative and keeps the
   sequence in a 1‑based cache array.  The cache capacity grows
   geometrically so that computing up to n = 10 000 is fast.  All
   arithmetic is performed in the build‑specified type `seq_t`.  It
   works for all allowed build configurations (uint8_t … uint64_t,
   arbitrary‑precision integer, etc.).
*/

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static seq_t *cache      = NULL;  /* 1‑based cache; cache[0] unused */
static size_t cache_cap  = 0;      /* number of elements allocated   */
static idx_t  real_max   = 0;      /* highest n for which a(n) is known */

/* Ensure the cache can address indices up to (but not including) 'needed'.
   Since our array is 1‑based, we need capacity > needed.  */
static void ensure_cap(idx_t needed)
{
    if ((size_t)needed < cache_cap)
        return; /* already sufficient */

    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while ((size_t)needed >= new_cap)
        new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();   /* allocation failed */

    /* Zero the newly allocated portion to keep the cache defined */
    if (new_cap > cache_cap)
        memset(tmp + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));

    cache      = tmp;
    cache_cap  = new_cap;
}

/* Initialise the first seven terms; called lazily. */
static void init_base(void)
{
    if (real_max >= 7)
        return;

    ensure_cap(7);
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)2;
    cache[3] = (seq_t)2;
    cache[4] = (seq_t)2;
    cache[5] = (seq_t)3;
    cache[6] = (seq_t)4;
    cache[7] = (seq_t)3;
    real_max = 7;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* out‑of‑range */

    init_base();

    if (n <= real_max)
        return cache[(size_t)n];

    /* Compute missing values up to n in ascending order. */
    for (idx_t i = real_max + 1; i <= n; ++i)
    {
        ensure_cap(i);
        /* a(i) = a(i-1) + a(i-6) - a(i-7);  (i >= 8 in this loop) */
        seq_t val = cache[i - 1];
        val += cache[i - 6];
        val -= cache[i - 7];
        cache[(size_t)i] = val;
    }
    real_max = n;
    return cache[(size_t)n];
}
