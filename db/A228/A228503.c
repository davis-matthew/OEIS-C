/*
 * A228503  Number of nX5 binary arrays with top left value 1 and no two ones
 *          adjacent horizontally, vertically, diagonally or antidiagonally.
 * https://oeis.org/A228503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 16*a(n-2) + a(n-3) - 27*a(n-4) + a(n-5) +
 *            4*a(n-6), with initial values a(1)=5, a(2)=11, a(3)=88,
 *            a(4)=358, a(5)=2002, a(6)=9528
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Indexing starts at 1 (a(1) is the first term). */

#include <stddef.h>   /* for size_t */

#define MAX_CACHED 10000

/* Seed values for the first six terms. */
static const seq_t _seed[6] = {
    (seq_t)5,   /* a(1) */
    (seq_t)11,  /* a(2) */
    (seq_t)88,  /* a(3) */
    (seq_t)358, /* a(4) */
    (seq_t)2002,/* a(5) */
    (seq_t)9528 /* a(6) */
};

/* Cache of computed terms; index 0 is unused. */
static seq_t _cache[MAX_CACHED + 1] = {0};
static idx_t _cached_up_to = 0;   /* largest index whose value is stored in _cache */

/* Compute a(n).  The function is safe for any order of calls and for
 * value types up to 128 bits or arbitrary‑precision types that are
 * default constructible. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, return zero */

    /* If the value is already cached, return it immediately. */
    if (n <= _cached_up_to)
        return _cache[n];

    /* Compute sequentially from the next missing index up to n. */
    idx_t i;
    for (i = _cached_up_to + 1; i <= n; ++i)
    {
        seq_t val;
        if (i <= 6)           /* use the known seeds */
            val = _seed[i - 1];
        else
        {
            /* Recurrence uses the six previous terms. */
            val = (2   * _cache[i - 1])        /* a(i-1) */
                + (16  * _cache[i - 2])        /* a(i-2) */
                + (1   * _cache[i - 3])        /* a(i-3) */
                - (27  * _cache[i - 4])        /* a(i-4) */
                + (1   * _cache[i - 5])        /* a(i-5) */
                + (4   * _cache[i - 6]);       /* a(i-6) */
        }
        _cache[i] = val;       /* store the computed value */
    }

    _cached_up_to = n;  /* update the highest cached index */
    return _cache[n];
}
