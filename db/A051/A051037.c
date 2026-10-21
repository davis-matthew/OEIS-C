/*
 * A051037  5-smooth numbers, i.e., numbers whose prime divisors are all <= 5.
 * https://oeis.org/A051037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 283, u32 n <= 1847, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates the 5‑smooth (Hamming) sequence: 
   a(1)=1, a(n+1) = smallest 2^i*3^j*5^k greater than a(n). */

#include <stdlib.h>

/* Static cache for previously computed terms */
static seq_t *cache = NULL;       /* array of sequence values */
static idx_t cache_len = 0;       /* number of terms stored so far */
static idx_t i2 = 0, i3 = 0, i5 = 0;  /* indices for next multiples */

seq_t a(idx_t n)
{
    /* Ensure the cache holds at least n terms */
    if (n == 0) return (seq_t)0;  /* undefined for n<1, safe guard */

    if (cache_len == 0) {
        /* initialise with the first term */
        cache = (seq_t*)malloc(sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache_len = 1;
        i2 = i3 = i5 = 0;
    }

    while (cache_len < n) {
        seq_t next2 = ((seq_t)2) * cache[i2];
        seq_t next3 = ((seq_t)3) * cache[i3];
        seq_t next5 = ((seq_t)5) * cache[i5];
        seq_t next = next2;
        if (next3 < next) next = next3;
        if (next5 < next) next = next5;

        /* Expand cache */
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * (cache_len + 1));
        cache[cache_len++] = next;

        if (next == next2) i2++;
        if (next == next3) i3++;
        if (next == next5) i5++;
    }

    return cache[n - 1];
}
