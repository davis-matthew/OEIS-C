/*
 * A110461  a(1) = 5; for n>1, if a(n-1) = nk + r, 0 <= r < n, then a(n) =
 *          k^(n-r)*(k+1)^r.
 * https://oeis.org/A110461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   seq_malloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=5; for n>1, let a(n-1)=n*k+r, 0≤r<n, then
   a(n)=k^(n−r)*(k+1)^r */
#include <stdbool.h>
#include "../utils/seq_malloc.h"
#include "../utils/seq_realloc.h"

/*------ Static cache for memoization -------------------------------------*/
/* cache[1]…cache[size] hold computed a(n) values. cache[0] is unused. */
static seq_t *cache = NULL;
static idx_t cap   = 0;   /* allocated length (>=size+1) */
static idx_t size  = 0;   /* highest n computed */

/* Ensure that cache contains indices up to need-1. */
static void ensure_capacity(idx_t need)
{
    if (need <= cap) return;

    idx_t new_cap = cap ? cap * 2 : 64;
    while (new_cap < need) new_cap <<= 1;

    if (cache == NULL)
        cache = seq_malloc(new_cap);
    else
        cache = seq_realloc(cache, new_cap);

    cap = new_cap;
}

/* Fast exponentiation for seq_t. */
static seq_t seq_pow(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

/*--------------------- Public function -----------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined for n < 1 */

    /* get enough space for index n */
    ensure_capacity(n + 1);           /* +1 for 1‑based indexing */

    /* initialise with a(1)=5 if cache empty */
    if (size == 0)
    {
        cache[1] = 5;
        size = 1;
    }

    /* compute missing terms up to n */
    if (n > size)
    {
        for (idx_t cur = size + 1; cur <= n; ++cur)
        {
            seq_t prev = cache[cur - 1];
            seq_t k = prev / (seq_t)cur;
            seq_t r = prev % (seq_t)cur;

            idx_t r_idx = (idx_t)r;          /* 0 ≤ r < cur */
            idx_t exp1 = cur - r_idx;
            idx_t exp2 = r_idx;

            seq_t part1 = seq_pow(k,     exp1);
            seq_t part2 = seq_pow(k + 1, exp2);
            cache[cur] = part1 * part2;
        }
        size = n;
    }

    return cache[n];
}
