/*
 * A144369  Shifts 5 places left under Dirichlet convolution.
 * https://oeis.org/A144369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  eigen, nonn
 * Formula:   a(1)=a(2)=a(3)=a(4)=a(5)=1, a(n) = Σ_{i·j = n-5} a(i)·a(j) for n
 *            > 5
 * Range:     u8 n <= 36, u16 n <= 76, u32 n <= 156, u64 n <= 316,
 *            u128 n <= 636, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* external types */
extern seq_t a(idx_t n);

/* 1‑based cache: cache[0] unused; cache[i] == 0  means “not computed”.
   All sequence values are positive, so zero is a safe uninitialized tag. */
static seq_t *seq_cache = NULL;
static size_t seq_cache_cap = 0;

/* Ensure that seq_cache can hold indices up to n.  Newly allocated
   entries are zeroed.  Works for both POD and arbitrary‑precision types
   because the zero value is assigned via an explicit conversion. */
static void
ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= seq_cache_cap)
        return;

    seq_t *new_cache = (seq_t *)realloc(seq_cache, needed * sizeof(seq_t));
    if (!new_cache)        /* allocation failure → leave the cache empty */
        return;

    seq_t zero = 0;        /* zero value usable for the sequence type */
    for (size_t i = seq_cache_cap; i < needed; ++i)
        new_cache[i] = zero;

    seq_cache   = new_cache;
    seq_cache_cap = needed;
}

/* Recursive computation of a(n).  The result is stored in the cache
   on the first computation. */
static seq_t
calc_a(idx_t n)
{
    if (n <= 5) {               /* initial values */
        ensure_cache(n);
        seq_cache[n] = 1;
        return 1;
    }

    ensure_cache(n);
    if (seq_cache[n] != 0)      /* already known */
        return seq_cache[n];

    idx_t m   = n - 5;
    seq_t sum = 0;

    for (idx_t d = 1; d * d <= m; ++d) {
        if (m % d == 0) {
            idx_t e   = m / d;
            seq_t ad  = calc_a(d);
            seq_t ae  = calc_a(e);
            seq_t prod = ad * ae;
            sum += prod;               /* pair (d,e) */
            if (d != e)
                sum += prod;           /* pair (e,d) */
        }
    }

    seq_cache[n] = sum;
    return sum;
}

/* External entry point required by the build. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined below the offset */
    return calc_a(n);
}
