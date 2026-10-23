/*
 * A164657  Denominators of partial sums of Theta(5) = Sum_{j>=1} 1/(2*j-1)^5.
 * https://oeis.org/A164657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator( sum_{j=1..n} 1/(2*j-1)^5 )
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t *nums = NULL;   /* numerators of partial sums */
static seq_t *dens = NULL;   /* denominators of partial sums (a(n)) */
static size_t cache_cnt = 0; /* number of stored partial sums, starting from n=0 */
static size_t cache_cap = 0; /* allocated size of the two arrays */

/* Grow the cached arrays to hold at least 'needed' elements (index 0..needed-1). */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = (cache_cap == 0) ? 64 : cache_cap << 1;
    while (new_cap < needed)
        new_cap <<= 1;

    /* realloc is safe for class types when the result is cast. */
    seq_t *new_nums = (seq_t*)realloc(nums, new_cap * sizeof(seq_t));
    seq_t *new_dens = (seq_t*)realloc(dens, new_cap * sizeof(seq_t));
    if (!new_nums || !new_dens)
        abort();  /* allocation failed */

    nums  = new_nums;
    dens  = new_dens;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* n is 1‑based. We store the partial sum for n=0 as 0/1. */
    if (n == 0)
        return (seq_t)1;  /* undefined but harmless */

    /* Initialise cache if empty. */
    if (cache_cnt == 0) {
        ensure_cache(1);
        nums[0] = (seq_t)0;
        dens[0] = (seq_t)1;
        cache_cnt = 1;
    }

    /* Compute terms up to n if not already cached. */
    for (idx_t i = (idx_t)cache_cnt; i <= n; ++i) {
        ensure_cache((size_t)i + 1);

        /* d = (2*i-1)^5 */
        seq_t base = (seq_t)((unsigned long long)(2 * i) - 1);
        seq_t d    = base * base;      /* base^2 */
        d          = d * d * base;     /* base^5 */

        /* add 1/d to the previous sum */
        seq_t prev_num = nums[i - 1];
        seq_t prev_den = dens[i - 1];

        seq_t new_num = prev_num * d + prev_den;
        seq_t new_den = prev_den * d;

        /* Reduce by gcd */
        seq_t g = gcd(new_num, new_den);
        if (g != (seq_t)1) {
            new_num /= g;
            new_den /= g;
        }

        nums[i] = new_num;
        dens[i] = new_den;
        cache_cnt = (size_t)i + 1;
    }

    return dens[n];
}
