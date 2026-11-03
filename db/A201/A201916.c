/*
 * A201916  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2+(x+2737)^2 = y^2.
 * https://oeis.org/A201916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 566, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 58 terms through n = 973 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A201916:
   a(n) = a(n-1) + 6*a(n-27) - 6*a(n-28) - a(n-54) + a(n-55),
   with the first 55 terms obtained by a brute‑force search of x.
   a(1) = 0, a(2) = 75 … as listed in the OEIS entry. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_i32.h"

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

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static seq_t *seq = NULL;      /* cached sequence values          */
static size_t seq_cap = 0;     /* allocated capacity of seq array   */
static size_t seq_len = 0;     /* number of terms currently cached */

/* Generate the first 55 terms by searching increasing x.
   The search stops once 55 non‑negative solutions of
   x^2 + (x+2737)^2 = y^2 have been found. */
static void init_first_fiftyfive(void)
{
    if (seq_len > 0)
        return;  /* already initialized */

    /* Allocate space for at least 55 terms. */
    seq_cap = 64;
    seq = seq_alloc(seq_cap);

    size_t found = 0;
    seq_t x = 0;
    while (found < 55) {
        seq_t xplus = x + (seq_t)2737;
        seq_t sum   = x * x + xplus * xplus;
        seq_t y     = floor_sqrt(sum);
        if (y * y == sum)
            seq[found++] = x;          /* store the x value */
        ++x;
    }
    seq_len = 55;
}

/* Ensure that at least n terms are present in the cache.
   If n > 55, compute the missing terms using the 55‑term recurrence. */
static void ensure_terms(idx_t n)
{
    init_first_fiftyfive();

    if ((size_t)n <= seq_len)
        return;   /* already available */

    /* Extend the array if necessary */
    if ((size_t)n > seq_cap) {
        size_t new_cap = seq_cap;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        seq = seq_realloc(seq, seq_cap, new_cap);
        seq_cap = new_cap;
    }

    /* Fill in the remaining terms */
    for (size_t i = seq_len; i < (size_t)n; ++i) {
        /* i is zero‑based index of a(i+1);
           recurrence requires previous 55 terms. */
        seq_t diff1 = seq[i - 27] - seq[i - 28];   /* a(i-26) - a(i-27)  */
        seq_t diff2 = seq[i - 54] - seq[i - 55];   /* a(i-53) - a(i-54)  */
        seq[i] = seq[i - 1] + (seq_t)6 * diff1 - diff2;
    }
    seq_len = (size_t)n;
}

/* Public interface defined by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0) /* offset protection – not used by the tests */
        return 0;
    ensure_terms(n);
    return seq[n - 1];
}
