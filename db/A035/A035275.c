/*
 * A035275  One sixth of deca-factorial numbers.
 * https://oeis.org/A035275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)*(10*n-4), a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t* __cache = NULL;          /* array of terms */
static size_t  __cache_cap = 0;        /* allocated capacity */
static idx_t   __cache_len = 0;        /* number of computed terms (>=1 if non‑empty) */

static void __ensure_cap(size_t needed)
{
    if (__cache_cap < needed) {
        size_t new_cap = (__cache_cap == 0) ? needed : (__cache_cap * 2);
        if (new_cap < needed) new_cap = needed;
#ifdef __cplusplus
        seq_t* new_arr = new seq_t[new_cap];
        for (idx_t i = 0; i < __cache_len; ++i) new_arr[i] = __cache[i];
        delete[] __cache;
        __cache = new_arr;
#else
        seq_t* new_arr = realloc(__cache, new_cap * sizeof(seq_t));
        __cache = new_arr;
#endif
        __cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* offset is 1, behaviour for 0 is irrelevant */

    if (__cache_len == 0) {         /* first time: initialise a(1) */
        __ensure_cap(1);
        __cache[0] = 1;             /* a(1) = 1 */
        __cache_len = 1;
    }

    if (n <= __cache_len) return __cache[n-1];

    __ensure_cap(n);

    for (idx_t i = __cache_len + 1; i <= n; ++i) {
        seq_t factor = (seq_t)(10*i - 4);
        seq_t val = __cache[i-2] * factor;   /* a(i) = a(i-1) * (10*i-4) */
        __cache[i-1] = val;
    }
    __cache_len = n;
    return __cache[n-1];
}
