/*
 * A053334  a(n) contains n digits (either '1' or '6') and is divisible by
 *          2^n.
 * https://oeis.org/A053334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = a(n-1) + 10^(n-1)*(6-5*(a(n-1)/2^(n-1) mod 2)), with a(1)
 *            = 6 (The digit at the beginning of a(n) is 6 if a(n-1) is
 *            divisible by 2^n, otherwise 1.)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t last_computed = 0;
static seq_t pow10_cur = 0;  /* 10^(last_computed) for the next term */

static void ensure_cache(idx_t n) {
    if (cache == NULL) {   /* first call */
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return; /* allocation failed – silently give up */
        cache[1] = (seq_t)6;
        last_computed = 1;
        pow10_cur = (seq_t)10;   /* for n=2 */
    }
    while (last_computed < n) {
        seq_t prev = cache[last_computed];
        seq_t bit = (prev >> last_computed) & (seq_t)1;   /* (a(n-1)/2^(n-1)) mod 2 */
        int start_digit = 6 - 5 * (int)bit;
        seq_t add = pow10_cur * (seq_t)start_digit;
        seq_t next = prev + add;

        /* grow table if necessary */
        if ((size_t)(last_computed + 1) >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return;          /* allocation failed – stop */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[++last_computed] = next;
        pow10_cur *= (seq_t)10;   /* prepare pow10 for the following n */
    }
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;   /* out of range; sequence is defined for n>=1 */
    ensure_cache(n);
    return cache[n];
}
