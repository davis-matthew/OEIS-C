/*
 * A067491  Powers of 5 with initial digit 1.
 * https://oeis.org/A067491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 *  A067491 – Powers of 5 whose first decimal digit is 1
 *
 *  a(n) is the n‑th integer 5^k such that the first digit of 5^k in base 10 is 1.
 *  i.e. 5^k ∈ [10^m , 2·10^m)  for some integer m.
 *
 *  Compute it on demand by generating successive powers of 5 and testing the
 *  leading digit via the fractional part of k·log10(5):
 *
 *      leading digit of 5^k = floor(10^frac(k·log10(5)))
 *
 *  We record the powers that satisfy the leading‑digit test in a static
 *  cache so repeated calls are O(1).  The algorithm is O(n) for n up to the
 *  requested index, and uses only seq_t arithmetic and long‑double math.
 */

#include <stdlib.h>
#include <math.h>

/* Static cache of computed terms */
static seq_t *cache = NULL;          /* array of terms */
static size_t cache_size = 0;        /* number of terms stored */
static size_t cache_capacity = 0;    /* allocated capacity */
static seq_t current_power = (seq_t)1; /* 5^k for the current exponent k */
static idx_t next_exponent = 1;      /* exponent to be processed next */

/* Ensure that cache contains at least n terms. */
static void ensure_cache(idx_t n)
{
    if ((size_t)n <= cache_size) return;   /* already have enough */

    /* Initialisation on first call */
    if (cache_size == 0) {
        /* start with 5^0 */
        cache_capacity = 4;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_capacity);
        cache[0] = current_power;   /* 5^0 = 1 */
        cache_size = 1;
        /* next_exponent is already 1 */
    }

    /* Constants needed for the leading‑digit test */
    const long double log10_5 = log10l((long double)5.0);
    const long double log10_2 = log10l((long double)2.0);
    const long double epsilon  = 1e-12L;  /* safety margin */

    while (cache_size < (size_t)n) {
        /* compute next power 5^k */
        current_power = (seq_t)(current_power * (seq_t)5);
        /* check if its leading digit is 1 */
        long double f = (long double)next_exponent * log10_5;
        long double frac = f - floorl(f);
        if (frac < (log10_2 - epsilon)) {
            /* grow array if needed */
            if (cache_size == cache_capacity) {
                cache_capacity *= 2;
                cache = (seq_t*)realloc(cache, sizeof(seq_t) * cache_capacity);
            }
            cache[cache_size++] = current_power;
        }
        next_exponent++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* out of range */
    ensure_cache(n);
    return cache[n-1];
}
