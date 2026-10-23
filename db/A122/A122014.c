/*
 * A122014  Expansion of
 *          -x*(8*x^7-33*x^6-30*x^5+88*x^4+35*x^3-33*x^2-11*x-1)/((x^4-x^3-3*x^2+x+1)*(x^4+x^3-3*x^2-x+1)).
 * https://oeis.org/A122014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A122014 – Expansion of 
    -x*(8*x^7-33*x^6-30*x^5+88*x^4+35*x^3-33*x^2-11*x-1)
    /((x^4-x^3-3*x^2+x+1)*(x^4+x^3-3*x^2-x+1))
    Formula: a(n) = 7*a(n-2) - 13*a(n-4) + 7*a(n-6) - a(n-8)   (n > 8)
    Initial terms: a(1)=1, a(2)=11, a(3)=40, a(4)=42,
                    a(5)=179, a(6)=181, a(7)=773, a(8)=790
    The function memoises computed values to allow out‑of‑order calls. */

#include <stdlib.h>

static seq_t *cache = NULL;      /* 1‑based indices, cache[0] unused */
static size_t cache_size = 0;    /* number of allocated slots (≥1)          */
static size_t computed_upto = 0; /* largest index whose value is known       */

/* Ensure the cache can hold indices up to 'need' (inclusive). */
static void ensure_cache_size(idx_t need)
{
    size_t k = (size_t)need;
    if (k + 1 <= cache_size)
        return;
    size_t new_cap = k + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                       /* allocation failure – terminate   */
    cache = tmp;
    cache_size = new_cap;
    /* new entries will be filled when computed */
}

/* Initialise the cache with the eleven known terms and set bounds. */
static void init_cache(void)
{
    if (cache_size >= 9)                /* already initialised              */
        return;

    ensure_cache_size(8);               /* need indices 0…8, i.e. 9 slots   */

    cache[0] = (seq_t)0;                /* a(0) = 0, used in recurrence    */
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)11;
    cache[3] = (seq_t)40;
    cache[4] = (seq_t)42;
    cache[5] = (seq_t)179;
    cache[6] = (seq_t)181;
    cache[7] = (seq_t)773;
    cache[8] = (seq_t)790;

    computed_upto = 8;
}

/* Compute a(n) with memoisation. */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;
    if (idx == 0)                       /* undefined for index 0            */
        return (seq_t)0;

    init_cache();

    /* Extend cache to hold the requested index, if necessary. */
    ensure_cache_size(idx);

    if (idx > computed_upto) {
        for (size_t i = computed_upto + 1; i <= idx; ++i) {
            seq_t term = 7 * cache[i - 2];
            term -= 13 * cache[i - 4];
            term += 7 * cache[i - 6];
            term -=  cache[i - 8];
            cache[i] = term;
        }
        computed_upto = idx;
    }

    return cache[idx];
}
