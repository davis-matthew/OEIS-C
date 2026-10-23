/*
 * A023362  a(n+1) = a(n) converted to base 5 from base 2 (written in base
 *          10).
 * https://oeis.org/A023362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=2; a(n+1)=∑_{k≥0} (bit_k(a(n)) * 5^k) (interpret binary
 *            digits of a(n) as base‑5 digits)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#ifdef __cplusplus
#include <new>
#endif

/* Convert a binary number into a base‑5 number using the same bit pattern */
static inline seq_t to_base5(seq_t x)
{
    seq_t res = 0;
    seq_t mult = 1;
    while (x) {
        res += (x & 1) * mult;
        mult *= 5;
        x >>= 1;
    }
    return res;
}

/* Cache of previously computed terms (array of pointers) */
static seq_t **cache_ptr = NULL;
static idx_t cap = 0;
static idx_t size = 0;

static void ensure_cache_capacity(idx_t needed)
{
    if (needed <= cap)
        return;
    idx_t new_cap = cap ? cap * 2 : 8;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t **new_arr = (seq_t**)malloc(new_cap * sizeof(seq_t*));
    if (!new_arr)
        abort();
    if (cache_ptr) {
        memcpy(new_arr, cache_ptr, cap * sizeof(seq_t*));
        free(cache_ptr);
    }
    cache_ptr = new_arr;
    cap = new_cap;
}

/* Public API: a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined in OEIS; safe guard */

    ensure_cache_capacity(n);

    /* initialise first term if it hasn't been stored yet */
    if (size == 0) {
        seq_t *ptr = (seq_t*)malloc(sizeof(seq_t));
        if (!ptr)
            abort();
#ifdef __cplusplus
        new (ptr) seq_t(2);
#else
        *ptr = 2;
#endif
        cache_ptr[0] = ptr;
        size = 1;
    }

    /* compute missing terms up to n */
    if (n > size) {
        for (idx_t i = size; i < n; ++i) {
            seq_t prev = *cache_ptr[i - 1];
            seq_t val = to_base5(prev);
            seq_t *ptr = (seq_t*)malloc(sizeof(seq_t));
            if (!ptr)
                abort();
#ifdef __cplusplus
            new (ptr) seq_t(val);
#else
            *ptr = val;
#endif
            cache_ptr[i] = ptr;
        }
        size = n;
    }

    return *cache_ptr[n - 1];
}
