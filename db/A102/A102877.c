/*
 * A102877  a(0) = 1, a(1) = 1; for n>0, a(2*n) = 3*a(2n-1), a(2*n+1) =
 *          3*a(2*n) - 2*a(n-1).
 * https://oeis.org/A102877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Compute OEIS A102877 recurrence: a(0)=1, a(1)=1; for i > 1: if i
 *            is even: a(i) = 3 * a(i-1); else a(i) = 3 * a(i-1) - 2 *
 *            a((i-1)/2 - 1); (i odd >1, with k = (i-1)/2 - 1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc/realloc/abort */

static seq_t *memo = NULL;      /* cache of computed values   */
static idx_t mem_size = 0;      /* current size of memo array */
static idx_t max_computed = 0;  /* highest index already filled */

/* enlarge memo so that indices up to needed exist */
static void ensure_capacity(idx_t needed)
{
    if (needed < mem_size)
        return;
    idx_t new_size = mem_size ? mem_size : 64;
    while (new_size <= needed)
        new_size <<= 1;

#ifdef __cplusplus
    /* C++: allocate with new so constructors run for non‑POD types   */
    seq_t *new_mem = new seq_t[new_size];
    if (memo) {
        for (idx_t i = 0; i < mem_size; ++i)
            new_mem[i] = memo[i];
        delete [] memo;
    }
    memo = new_mem;
#else
    /* C: plain realloc is fine for integer seq_t                  */
    seq_t *new_mem = (seq_t *)realloc(memo, new_size * sizeof(seq_t));
    if (!new_mem)
        abort();               /* memory exhaustion is fatal */
    memo = new_mem;
#endif
    mem_size = new_size;
}

seq_t a(idx_t n)
{
    /* ensure we have a space for 0 and 1 at the very least   */
    if (max_computed < 1)
    {
        ensure_capacity(1);
        memo[0] = (seq_t)1;
        memo[1] = (seq_t)1;
        max_computed = 1;
    }

    if (n > max_computed)
    {
        ensure_capacity(n);
        for (idx_t i = max_computed + 1; i <= n; ++i)
        {
            if ((i & 1) == 0)        /* even */
            {
                memo[i] = (seq_t)3 * memo[i-1];
            }
            else                     /* odd, i > 1 */
            {
                /* k = (i-1)/2 - 1   */
                idx_t k = ((i - 1) >> 1) - 1;
                memo[i] = (seq_t)3 * memo[i-1] - (seq_t)2 * memo[k];
            }
        }
        max_computed = n;
    }
    return memo[n];
}
