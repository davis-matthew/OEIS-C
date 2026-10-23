/*
 * A139370  Let the binary expansion of n be n = Sum_{k} 2^{r_k}, let e(n) be
 *          the number of r_k's that are even, o(n) the number that are odd;
 *          sequence lists n such that e(n) < o(n).
 * https://oeis.org/A139370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A139370
 *  a(n) = the n-th positive integer whose binary representation contains
 *  strictly more set bits on odd-positioned places (1,3,5,…) than on
 *  even-positioned places (0,2,4,…).
 *  This function generates the sequence on demand, keeping a static
 *  cache so that calls with arbitrary n are answered efficiently.
 */

#include <stdint.h>
#include <stdlib.h>

/* Forward declaration of this function */
seq_t a(idx_t n);

/* Static cache of already found sequence values */
static seq_t *cache = NULL;     /* array holding the sequence values */
static size_t cache_cap = 0;    /* current allocation size (in elements) */
static size_t cache_len = 0;    /* number of elements stored */
static uint64_t next_check = 1; /* next integer to test */

/* Grow the cache array to at least 'needed' elements */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* In a production/contest environment you may handle realloc failure,
     * but here we simply assume it succeeds because the required size is
     * modest (for n ≈ 10 000 the 10 000th term is < 200 000). */
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Count number of set bits in odd and even positions of x.
   x is at most 64 bits in the intended use range (n ≲ 10⁶ for 10 000 terms). */
static void count_odd_even(uint64_t x, int *odd, int *even)
{
    *odd = *even = 0;
    while (x)
    {
        int pos = __builtin_ctzll(x);  /* position of least‑significant set bit */
        if (pos & 1)
            (*odd)++;
        else
            (*even)++;
        x &= x - 1;                    /* clear that bit */
    }
}

/* Main sequence generator */
seq_t a(idx_t n)
{
    /* Lazy initialization: fill cache until we have at least 'n' entries */
    while (cache_len < n)
    {
        /* Check current integer: count odd/even bits */
        int odd, even;
        count_odd_even(next_check, &odd, &even);
        if (even < odd)   /* stricly more odd bits than even */
        {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)next_check;
        }
        next_check++;
    }
    return cache[n - 1];
}
