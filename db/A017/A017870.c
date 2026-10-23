/*
 * A017870  Expansion of 1/(1-x^8-x^9-x^10-x^11-x^12).
 * https://oeis.org/A017870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1..7)=0, a(8..12)=1,
 *            a(n)=a(n-8)+a(n-9)+a(n-10)+a(n-11)+a(n-12) for n>11
 * Range:     u8 n <= 46, u16 n <= 81, u32 n <= 149, u64 n <= 285,
 *            u128 n <= 556, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* dynamically growing array of a(n) values */
static size_t cap = 0;        /* current capacity of cache (elements) */
static idx_t last_computed = 0; /* greatest index that has been filled */

seq_t a(idx_t n)
{
    /* --- Allocate or grow storage as needed --------------------------------- */
    if (cache == NULL) {
        /* first call: allocate space for at least n+1 elements */
        cap = (size_t)n + 1;
        if (cap == 0) cap = 1;                     /* guard against n==0 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;        /* out‑of‑memory: return 0 */
        cache[0] = (seq_t)1;                       /* a(0)=1 */
    } else if (cap <= (size_t)n) {
        /* enlarge capacity when n exceeds current size */
        size_t new_cap = cap * 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;          /* out‑of‑memory */
        cache = tmp;
        cap = new_cap;
    }

    /* --- Compute missing values up to n ------------------------------------- */
    if (last_computed < n) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            if (i <= 7) {
                cache[i] = (seq_t)0;
            } else if (i <= 12) {
                cache[i] = (seq_t)1;
            } else {
                cache[i] = cache[i-8] + cache[i-9]
                         + cache[i-10] + cache[i-11] + cache[i-12];
            }
        }
        last_computed = n;
    }

    return cache[n];
}
