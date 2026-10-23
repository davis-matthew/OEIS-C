/*
 * A045777  a(1)=1, a(2)=2; thereafter successive products of pairs of digits
 *          make further digits.
 * https://oeis.org/A045777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9168, u16 n <= 9168, u32 n <= 9168, u64 n <= 9168,
 *            u128 n <= 9168, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=2.
   For every k ≥ 1 take the product of the k‑th and (k+1)‑st digits
   of the sequence, write that product in base 10 (most significant
   digit first), and append its decimal digits to the sequence.
   The following term a(n) is the n‑th digit produced by this rule. */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* digits of the sequence (a(1), a(2), …) */
static size_t cache_len = 0;     /* number of stored digits          */
static size_t cache_cap = 0;     /* allocated capacity               */
static size_t next_pair = 0;     /* index of the first digit of next pair */

/* Grow the cache to hold at least `needed` digits. */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed)
        new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Primary sequence function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined below offset */

    size_t N = (size_t)n;           /* good because n ≤ about 10 000 */

    /* Initialise first two terms once. */
    if (cache_len < 2) {
        ensure_cache(2);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache_len = 2;
        next_pair = 0;
    }

    /* Generate digits until we have at least N of them. */
    while (cache_len < N) {
        /* The pair to multiply: cache[next_pair] and cache[next_pair+1]. */
        if (next_pair + 1 >= cache_len)
            break;                   /* safety; should never happen */

        seq_t d1   = cache[next_pair];
        seq_t d2   = cache[next_pair + 1];
        seq_t prod = d1 * d2;       /* 0 ≤ prod ≤ 81, fits in seq_t */

        if (prod >= (seq_t)10) {
            seq_t high = prod / (seq_t)10;
            seq_t low  = prod % (seq_t)10;
            ensure_cache(cache_len + 2);
            cache[cache_len++] = high;
            cache[cache_len++] = low;
        } else {
            ensure_cache(cache_len + 1);
            cache[cache_len++] = prod;
        }
        next_pair++;                /* move to the next pair */
    }

    return cache[N - 1];
}
