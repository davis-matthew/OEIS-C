/*
 * A015465  q-Fibonacci numbers for q=8, scaling a(n-2).
 * https://oeis.org/A015465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 8^(n-2) * a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 58 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

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

static seq_t *seq = NULL;    /* cached sequence values */
static size_t seq_cap = 0;   /* number of values stored */
static seq_t pow8 = 0;       /* 8^(seq_cap-2), ready for next term */

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (seq == NULL) {
        seq_cap = 2;            /* a(0) and a(1) */
        seq = seq_alloc(seq_cap);
        seq[0] = 0;
        seq[1] = 1;
        pow8  = 1;              /* 8^(2-2) = 1 */
    }

    /* extend cache if needed */
    size_t required = (size_t)n + 1;
    if (required > seq_cap) {
        size_t old_cap = seq_cap;
        seq_cap = required;
        seq = seq_realloc(seq, old_cap, seq_cap);

        for (idx_t i = old_cap; i <= n; ++i) {
            seq[(size_t)i] = seq[(size_t)i-1] + pow8 * seq[(size_t)i-2];
            pow8 *= 8;          /* update for next index */
        }
    }

    return seq[(size_t)n];
}
