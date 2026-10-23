/*
 * A183556  Positions of the records of the negative integers in A179319; a(n)
 *          is the first position in A179319 equal to -n.
 * https://oeis.org/A183556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = A059973(2*n + (n&1)) - 1
 * 
 * A059973 is defined by
 *   a(0)=0, a(1)=1, a(2)=1, a(3)=2,
 *   a(k)=4*a(k-2)+a(k-4)   for k>=4.
 *
 * The function memoises A059973 up to the index needed for a given n.
 */

#include <stdlib.h>

/* Static cache for A059973 values */
static seq_t *A059cache = NULL;
static idx_t A059cache_len = 0;

/* Ensure A059cache contains values up to index `limit` (inclusive) */
static void _ensure_A059cache(idx_t limit)
{
    if (limit < A059cache_len)
        return;                       /* already computed */

    idx_t old_len = A059cache_len;
    A059cache = (seq_t *)realloc(A059cache, (limit + 1) * sizeof(seq_t));
    if (A059cache == NULL) {
        /* Allocation failed – terminate */
        exit(1);
    }

    for (idx_t i = old_len; i <= limit; ++i) {
        if (i == 0)
            A059cache[0] = 0;
        else if (i == 1)
            A059cache[1] = 1;
        else if (i == 2)
            A059cache[2] = 1;
        else if (i == 3)
            A059cache[3] = 2;
        else
            A059cache[i] = A059cache[i - 2] * (seq_t)4 + A059cache[i - 4];
    }
    A059cache_len = limit + 1;
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    /* n >= 1 by the sequence definition */
    idx_t idx = 2 * n + (n & 1);   /* 2n if n even, 2n+1 if n odd */

    _ensure_A059cache(idx);

    return A059cache[idx] - 1;
}
