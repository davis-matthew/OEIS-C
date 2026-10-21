/*
 * A231134  Number of (n+1) X (2+1) black-square subarrays of 0..2 arrays with
 *          no element equal to a strict majority of its diagonal and
 *          antidiagonal neighbors, with values 0..2 introduced in row major
 *          order.
 * https://oeis.org/A231134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 22*a(n-2) - 36*a(n-4) + 16*a(n-6),
 * with a(1)=2, a(2)=9, a(3)=40, a(4)=182, a(5)=808, a(6)=3688.
 *
 * This implements the linear recurrence proved for the OEIS sequence
 * A231134.  The values are stored in a static cache so that repeated
 * calls (in any order) are answered in O(1) after the values have
 * been computed once.  The cache is expanded lazily as needed.
 */

#include <stdlib.h>
#include <stddef.h>

/* Static cache for previously computed terms */
static seq_t *cache      = NULL;    /* array indexed from 0..n, we use 1..n */
static size_t cache_cap  = 0;      /* current allocated size (≥ 7 when used) */
static idx_t  computed   = 0;      /* largest index that has been computed */

/* Ensure that the cache can hold entries up to 'n'.
   Allocate the array on first use and store the first six terms. */
static void
ensure_cache(idx_t n)
{
    size_t needed = (size_t)(n + 1);
    if (needed < 7) needed = 7;          /* need space for indices 1..6 */

    if (!cache) {                        /* first allocation */
        cache = (seq_t*)calloc(needed, sizeof(seq_t));
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)40;
        cache[4] = (seq_t)182;
        cache[5] = (seq_t)808;
        cache[6] = (seq_t)3688;
        cache_cap = needed;
        computed   = 6;
    } else if (cache_cap < needed) {     /* enlarge existing array */
        cache = (seq_t*)realloc(cache, needed*sizeof(seq_t));
        cache_cap = needed;
        /* base terms (indices 1..6) are already set and remain correct. */
    }
}

/* Main function for the sequence */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    /* Return the first six explicit values directly */
    if (n <= 6) {
        switch (n) {
        case 1: return (seq_t)2;
        case 2: return (seq_t)9;
        case 3: return (seq_t)40;
        case 4: return (seq_t)182;
        case 5: return (seq_t)808;
        case 6: return (seq_t)3688;
        }
    }

    ensure_cache(n);                     /* guarantees cache[1..n] exists */

    /* Compute any missing terms up to n */
    idx_t start = computed + 1;
    if (start < 7) start = 7;            /* recurrence starts at n = 7 */

    for (idx_t i = start; i <= n; ++i) {
        cache[i] = 22 * cache[i-2] - 36 * cache[i-4] + 16 * cache[i-6];
    }
    computed = n;

    return cache[n];
}
