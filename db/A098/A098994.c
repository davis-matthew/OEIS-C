/*
 * A098994  Number of permutations of [n] with exactly 3 descents which avoid
 *          the pattern 1324.
 * https://oeis.org/A098994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 30, u64 n <= 242,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) satisfies the recurrence
 *
 *   a(1)=0, a(2)=0, a(3)=0, a(4)=1,
 *   a(5)=26, a(6)=229, a(7)=1246, a(8)=5086,
 *   a(9)=17084, a(10)=49768, a(11)=129958, a(12)=311051;
 *
 *   For n>12:
 *     a(n)=12*a(n-1)-66*a(n-2)+220*a(n-3)-495*a(n-4)+792*a(n-5)
 *          -924*a(n-6)+792*a(n-7)-495*a(n-8)+220*a(n-9)-66*a(n-10)
 *          +12*a(n-11)-a(n-12);
 *
 * This function memoises the sequence so that each value is
 * computed at most once, regardless of call order.
 */

#include <stdlib.h>
#include <stddef.h>

extern seq_t a(idx_t);   /* prototype used by build scripts */

/* internal cache */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of values stored so far */
static size_t cache_cap = 0;   /* allocated capacity */

/* first 12 values, cast to seq_t to avoid too‑wide literals */
static const seq_t init_12[12] = {
    (seq_t)0,  (seq_t)0,  (seq_t)0,  (seq_t)1,
    (seq_t)26, (seq_t)229,(seq_t)1246,(seq_t)5086,
    (seq_t)17084,(seq_t)49768,(seq_t)129958,(seq_t)311051
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset not defined below 1 */

    /* convert to a (safe) size_t for array indexing */
    size_t need = (size_t)n;

    /* grow cache if necessary */
    if (cache == NULL || cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < need) new_cap <<= 1;   /* double until sufficient */
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) return 0;                 /* allocation failure */
        cache = new_ptr;
        cache_cap = new_cap;
    }

    /* initialise base values if not yet done */
    if (cache_len < 12) {
        size_t up_to = need < 12 ? need : 12;
        for (size_t i = cache_len; i < up_to; ++i)
            cache[i] = init_12[i];
        cache_len = up_to;
    }

    /* compute terms up to the desired index using recurrence */
    size_t start = cache_len;
    if (start < 12) start = 12;         /* recurrence starts at a(13) */
    for (size_t i = start; i < need; ++i) {
        /* i corresponds to a(i+1); cache indices are 0‑based */
        seq_t pos = 0;
        pos += (seq_t)12   * cache[i-1];
        pos += (seq_t)220  * cache[i-3];
        pos += (seq_t)792  * cache[i-5];
        pos += (seq_t)792  * cache[i-7];
        pos += (seq_t)220  * cache[i-9];
        pos += (seq_t)12   * cache[i-11];

        seq_t neg = 0;
        neg += (seq_t)66   * cache[i-2];
        neg += (seq_t)495  * cache[i-4];
        neg += (seq_t)924  * cache[i-6];
        neg += (seq_t)495  * cache[i-8];
        neg += (seq_t)66   * cache[i-10];
        neg +=                 cache[i-12];

        cache[i] = pos - neg;   /* pos ≥ neg for the valid sequence */

        /* safety: if the intermediate wrapped due to unsigned overflow,
           the final result is still correct for values that fit into seq_t. */
    }

    /* update the length of stored values */
    if (cache_len < need) cache_len = need;

    return cache[need - 1];
}
