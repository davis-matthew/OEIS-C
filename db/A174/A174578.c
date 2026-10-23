/*
 * A174578  Expansion of g.f.: -1/(-1 + x + x^4 - x^10 + x^13 + x^14).
 * https://oeis.org/A174578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-4)-a(n-10)+a(n-13)+a(n-14) * initial values:
 *            a(0)=1, a(1)=1, a(2)=1, a(3)=1, * a(4)=2, a(5)=3, a(6)=4, a(7)=5
 *            ... * (indices are 0‑based; a(0) is the first term in the OEIS
 *            list)
 * Range:     u8 n <= 19, u16 n <= 37, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#  include <new>
#else
#  include <stdlib.h>
#endif

static seq_t *cache = NULL;   /* stored values, index 0 .. cache_size-1 */
static idx_t   cache_size = 0;/* number of stored values */

/* compute a(n) for any n >= 0 */
seq_t a(idx_t n)
{
    /* enlarge cache if needed */
    idx_t old_size = cache_size;
    idx_t new_size = n + 1;
    if (new_size > old_size) {
#ifdef __cplusplus
        /* allocate a larger array and copy existing values */
        seq_t *new_cache = new seq_t[new_size];
        for (idx_t i = 0; i < old_size; ++i) new_cache[i] = cache[i];
        if (cache) delete[] cache;
        cache = new_cache;
#else
        /* realloc for C builds */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;  /* allocation failed; return 0 */
        cache = new_cache;
#endif
        cache_size = new_size;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = old_size; i < cache_size; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else {
            seq_t val = cache[i - 1];
            if (i >= 4)   val += cache[i - 4];
            if (i >= 10)  val -= cache[i - 10];
            if (i >= 13)  val += cache[i - 13];
            if (i >= 14)  val += cache[i - 14];
            cache[i] = val;
        }
    }

    return cache[n];
}
