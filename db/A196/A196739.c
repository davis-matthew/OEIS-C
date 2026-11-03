/*
 * A196739  a(n) = n! - n^10.
 * https://oeis.org/A196739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = factorial(n) - n^10
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 8, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 400 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_u64_idx.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* Cache factorial values so that repeated calls are fast. */
    static seq_t *fac = NULL;
    static idx_t fac_len = 0;   /* current allocated length */
    static idx_t fac_last = 0;  /* largest n for which fac[n] is computed */

    if (fac == NULL) {                     /* first call: allocate and init */
        fac_len = n + 1;                   /* at least needed length */
        fac = seq_alloc(fac_len);
        fac[0] = (seq_t)1;                 /* 0! = 1 */
        fac_last = 0;
    }

    /* Grow the cache if required and compute new factorials. */
    if (n > fac_last) {
        idx_t new_len = n + 1;
        if (new_len > fac_len) {
            fac = seq_realloc(fac, fac_len, new_len);
            fac_len = new_len;
        }
        for (idx_t i = fac_last + 1; i <= n; ++i) {
            fac[i] = fac[i - 1] * (seq_t)i;
        }
        fac_last = n;
    }

    seq_t fact = fac[n];
    seq_t power = ipow_u64_idx((uint64_t)n, (idx_t)10);
    return fact - power;
}
