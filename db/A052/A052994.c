/*
 * A052994  Expansion of 2x(1-x)/(1-2x-x^2+x^3).
 * https://oeis.org/A052994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=2, a(2)=2,
   a(n)=a(n-2)+2*a(n-1)-a(n-3) for n≥3 */
#include <stdlib.h>
#include <stddef.h>

static seq_t *cache   = NULL;  /* array of computed terms */
static size_t capacity = 0;    /* allocated capacity of cache   */
static size_t max_computed = 0;/* last index that has a value   */

/* Ensure that cache can hold indices 0 … need.  */
static void ensure_capacity(size_t need)
{
    size_t new_cap = capacity;
    if (new_cap == 0) new_cap = 4;
    while (new_cap <= need) {
        new_cap <<= 1;                /* grow by powers of two   */
    }
    if (new_cap > capacity) {
#ifdef __cplusplus
        seq_t *tmp = new seq_t[new_cap];
        for (size_t i = 0; i < capacity; ++i) tmp[i] = cache[i];
        delete[] cache;
        cache = tmp;
#else
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
#endif
        capacity = new_cap;
    }
}

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;          /* index for array access */

    /* handle trivial base values without allocating memory */
    if (idx == 0) return (seq_t)0;
    if (idx == 1) return (seq_t)2;
    if (idx == 2) return (seq_t)2;

    /* initialize cache on first call */
    if (cache == NULL) {
        ensure_capacity(2);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)2;
        max_computed = 2;
    }

    /* extend computation up to idx if necessary */
    if (idx > max_computed) {
        ensure_capacity(idx);
        for (size_t i = max_computed + 1; i <= idx; ++i) {
            cache[i] = cache[i-2] + (seq_t)2 * cache[i-1] - cache[i-3];
        }
        max_computed = idx;
    }

    return cache[idx];
}
