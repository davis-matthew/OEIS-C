/*
 * A029132  Expansion of 1/((1-x)(1-x^9)(1-x^10)(1-x^11)).
 * https://oeis.org/A029132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   The generating function is 1/((1-x)(1-x^9)(1-x^10)(1-x^11)),
 *            giving the linear recurrence a(n) = a(n-1) + a(n-9) - a(n-12) -
 *            a(n-19) + a(n-22) + a(n-30) - a(n-31) for n ≥ 1, with a(0)=1 and
 *            a(k)=0 for k<0
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include standard headers */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static seq_t *a_cache = NULL;   /* array holding computed a(n) */
static size_t a_cache_cap = 0;  /* allocated size of a_cache      */
static size_t a_computed = 0;   /* highest index that has been
                                  computed (inclusive)            */
static bool a_initialized = false;

/* Ensure that a_cache has capacity for indices 0 … needed-1.
   Handles both C and C++ where seq_t may be a class. */
static void ensure_capacity(size_t needed)
{
    if (needed <= a_cache_cap) return;

    size_t new_cap = needed;
    seq_t *new_cache;

#if defined(__cplusplus)
    new_cache = new seq_t[new_cap];
    if (!new_cache) exit(EXIT_FAILURE);
    for (size_t i = 0; i < a_cache_cap; ++i) new_cache[i] = a_cache[i];
    for (size_t i = a_cache_cap; i < new_cap; ++i) new_cache[i] = 0;
    delete[] a_cache;
#else
    new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);
    if (a_cache) memcpy(new_cache, a_cache, a_cache_cap * sizeof(seq_t));
    for (size_t i = a_cache_cap; i < new_cap; ++i) new_cache[i] = 0;
    free(a_cache);
#endif
    a_cache = new_cache;
    a_cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;
    ensure_capacity(idx + 1);

    if (!a_initialized) {
        /* set the single known initial value */
        a_cache[0] = 1;
        a_computed = 0;
        a_initialized = true;
    }

    if (idx <= a_computed) return a_cache[idx];

    /* compute missing terms up to idx using the recurrence */
    for (size_t i = a_computed + 1; i <= idx; ++i) {
        seq_t val = 0;
        val += a_cache[i - 1];           /* a(n-1)   */
        if (i >= 9)  val += a_cache[i - 9];   /* a(n-9)   */
        if (i >= 12) val -= a_cache[i - 12];  /* a(n-12)  */
        if (i >= 19) val -= a_cache[i - 19];  /* a(n-19)  */
        if (i >= 22) val += a_cache[i - 22];  /* a(n-22)  */
        if (i >= 30) val += a_cache[i - 30];  /* a(n-30)  */
        if (i >= 31) val -= a_cache[i - 31];  /* a(n-31)  */
        a_cache[i] = val;
    }
    a_computed = idx;
    return a_cache[idx];
}
