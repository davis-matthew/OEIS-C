/*
 * A038494  Images of hexamorphic numbers: suppose k-th hexagonal number H(k)
 *          (A000384) ends in k; sequence gives positive values of H(k).
 * https://oeis.org/A038494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = k (2k − 1); k is the n-th positive integer for which
 *            2k(k−1) is divisible by 10^digits(k), i.e. the hexagonal number
 *            H(k) ends with the decimal digits of k
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static seq_t *cache = NULL;   /* cached sequence values          */
static size_t cache_cap = 0;  /* capacity of the cache array      */
static size_t cache_len = 0;  /* number of stored terms            */

static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = ptr;
    cache_cap = new_cap;
}

/* The variables below keep the state of the incremental search for the
   next qualifying k.  They are static so that successive calls to a()
   continue from where the previous call left off. */
static idx_t next_k = 1;   /* next candidate k to test              */
static uint64_t pow10 = 10;/* 10^digits(next_k)                      */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;                     /* undefined, guard */

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n)
    {
        /* Update the power of ten when the digit length changes */
        if (next_k >= pow10)
            pow10 *= 10U;                     /* next power of 10   */

        /* Compute H(k) = k*(2k-1) */
        seq_t H = (seq_t)next_k * (((seq_t)2 * (seq_t)next_k) - 1);

        /* Check the last-digits condition: H ≡ k (mod 10^d) */
        if ((H % (seq_t)pow10) == (seq_t)next_k)
        {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = H;
        }

        ++next_k;   /* advance to the next candidate */
    }

    return cache[(size_t)(n - 1)];
}
