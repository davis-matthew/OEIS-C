/*
 * A051515  (Terms in A014738)/4.
 * https://oeis.org/A051515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-2) - 4*a(n-3) - 6*a(n-4) + 6*a(n-5) +
 *            4*a(n-6) - 4*a(n-7) - a(n-8) + a(n-9) for n > 9 Initial values
 *            (offset 1): a(1)= 9, a(2)=25, a(3)=196, a(4)=324, a(5)=1089,
 *            a(6)=1521, a(7)=3600, a(8)=4624, a(9)=9025
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* array of cached values (1-indexed) */
static idx_t  seq_cache_size = 0; /* allocated size (including index 0) */
static idx_t  seq_cache_max  = 0; /* highest n cached (0 if none) */

/* Ensure the cache can hold indices up to n */
static void seq_cache_ensure(idx_t n)
{
    if (seq_cache_size <= n) {
        idx_t new_size = n + 1; /* allocate one extra for index 0 */
        seq_t *new_cache = (seq_t *)realloc(seq_cache, new_size * sizeof(seq_t));
        seq_cache = new_cache;
        seq_cache_size = new_size;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined, return 0 for safety */

    /* Expand cache if needed and initialise the first 9 terms once */
    if (seq_cache == NULL || n >= seq_cache_size) {
        seq_cache_ensure(n > 9 ? n : 9);
    }
    if (seq_cache_max < 9) {
        seq_cache[1] = 9;
        seq_cache[2] = 25;
        seq_cache[3] = 196;
        seq_cache[4] = 324;
        seq_cache[5] = 1089;
        seq_cache[6] = 1521;
        seq_cache[7] = 3600;
        seq_cache[8] = 4624;
        seq_cache[9] = 9025;
        seq_cache_max = 9;
    }

    /* If requested value already cached, return it immediately */
    if (n <= seq_cache_max) return seq_cache[n];

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = seq_cache_max + 1; i <= n; ++i) {
        if (i <= 9) continue;  /* already initialised */

        seq_t pos = seq_cache[i-1];
        pos += seq_cache[i-2] * 4;
        pos += seq_cache[i-5] * 6;
        pos += seq_cache[i-6] * 4;
        pos += seq_cache[i-9];

        seq_t neg = seq_cache[i-3] * 4;
        neg += seq_cache[i-4] * 6;
        neg += seq_cache[i-7] * 4;
        neg += seq_cache[i-8];

        seq_cache[i] = pos - neg;   /* pos ≥ neg, so no unsigned underflow */
    }

    seq_cache_max = n;
    return seq_cache[n];
}
