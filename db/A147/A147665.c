/*
 * A147665  a(n) = a(a(n - 1)) + r(n) for n >= 3, where r(3*k) = a(a(k)),
 *          r(3*k+1) = a(a(k)) and r(3*k+2) = a(n-a(k)), with a(0) = 0 and
 *          a(1) = a(2) = 1.
 * https://oeis.org/A147665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=a(2)=1, a(n)=a(a(n-1))+r(n) for n≥3 r(3k)=a(a(k)),
 *            r(3k+1)=a(a(k)), r(3k+2)=a(n-a(k)). The code memoises the
 *            values; it works for arbitrary call order
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array holding computed terms */
static idx_t  cache_len = 0;     /* allocated length of the array   */
static idx_t  computed_upto = 2; /* highest index that has been computed */

/* Return the nth term of the sequence */
seq_t a(idx_t n)
{
    /* Handle the initial values directly */
    if (n == 0) return (seq_t)0;
    if (n == 1 || n == 2) return (seq_t)1;

    /* Initialise the cache on first use */
    if (cache == NULL) {
        cache_len = 3;
        cache = (seq_t *)calloc((size_t)cache_len, sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;      /* allocation failed   */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        computed_upto = 2;
    }

    /* Ensure the cache has space for the desired index */
    if (n + 1 > cache_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc((void *)cache, new_len * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;          /* allocation failed   */
        cache = tmp;
        cache_len = (idx_t)new_len;
    }

    /* Compute terms only up to n if they are not yet present */
    for (idx_t i = computed_upto + 1; i <= n; ++i)
    {
        /* a(i-1) and a(a(i-1)) */
        seq_t a_prev   = cache[i - 1];                   /* a(i-1)   */
        seq_t a_a_prev = cache[(idx_t)a_prev];           /* a(a(i-1)) */

        /* k = floor(i/3), ak = a(k) */
        idx_t k  = i / 3;
        seq_t ak = cache[k];

        /* Compute r(i) according to the residue of i mod 3 */
        seq_t r;
        if (i % 3 == 0 || i % 3 == 1) {
            /* r = a(a(k)) */
            r = cache[(idx_t)ak];
        } else {
            /* r = a(i - a(k))  */
            idx_t idx = i - (idx_t)ak;
            r = cache[idx];
        }

        /* a(i) = a(a(i-1)) + r(i) */
        cache[i] = a_a_prev + r;
    }

    computed_upto = n;   /* remember what has been computed */
    return cache[n];
}
