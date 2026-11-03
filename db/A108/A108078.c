/*
 * A108078  Determinant of a Hankel matrix with factorial elements.
 * https://oeis.org/A108078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+1)! * Product_{i=1..n-1} (i+1)! * (n-i)!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Validated: standard, pass, 27 terms through n = 30 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Allocate an array of seq_t, compatible with C and C++ */
static inline seq_t *alloc_seq_array(size_t cap)
{
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t *)malloc(cap * sizeof(seq_t));
#endif
}

/* Free an array of seq_t, compatible with C and C++ */
static inline void free_seq_array(seq_t *ptr)
{
#ifdef __cplusplus
    delete [] ptr;
#else
    free(ptr);
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* compute factorials up to (n+1)! */
    size_t sz = (size_t)(n + 2);                 /* indices 0 .. n+1 */
    seq_t *fact = alloc_seq_array(sz);
    if (!fact) return (seq_t)0;

    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= n + 1; ++i) {
        fact[i] = fact[i-1] * (seq_t)i;
    }

    seq_t res = fact[n+1];
    for (idx_t i = 1; i <= n - 1; ++i) {
        res *= fact[i+1];
        res *= fact[n-i];
    }

    free_seq_array(fact);
    return res;
}
