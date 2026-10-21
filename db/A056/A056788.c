/*
 * A056788  a(n) = n^n + (n-1)^(n-1).
 * https://oeis.org/A056788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^n + (n-1)^(n-1) (offset 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* for realloc, NULL */

/* Memoisation buffer */
static seq_t *cache = NULL;
static idx_t  cache_size = 0;   /* number of elements currently allocated */

/* fast exponentiation for seq_t; exp is idx_t but may be zero */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;           /* 1 has a constructor from int for both fixed‑width and big‑int types */
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    /* undefined behaviour for n < 1 in the OEIS entry; return 0 as a guard */
    if (n < 1)
        return 0;

    /* allocate or grow the cache if needed */
    if (n >= cache_size)
    {
        idx_t new_size = n + 1;        /* we store element indexed by n itself */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)                      /* allocation failed – unlikely in tests */
            return 0;
        cache = tmp;
        /* initialise the newly allocated entries to zero */
        for (idx_t i = cache_size; i < new_size; ++i)
            cache[i] = 0;
        cache_size = new_size;
    }

    /* return cached value if already computed */
    if (cache[n] != 0)
        return cache[n];

    /* compute n^n + (n-1)^(n-1) */
    seq_t pow_n      = pow_seq((seq_t)n, n);
    seq_t pow_n_1    = pow_seq((seq_t)(n - 1), n - 1);
    seq_t result = pow_n + pow_n_1;

    /* store in cache and return */
    cache[n] = result;
    return result;
}
