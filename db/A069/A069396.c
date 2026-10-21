/*
 * A069396  Half the number of 3 X n binary arrays with a path of adjacent 1's
 *          and a path of adjacent 0's from top row to bottom row.
 * https://oeis.org/A069396
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) satisfies the linear recurrence of order 5
 * a(n) = 21*a(n-1) - 152*a(n-2) + 424*a(n-3) - 328*a(n-4) + 64*a(n-5)
 * for n ≥ 7.
 *
 * The first five terms (a(2) … a(6)) are obtained from the Taylor
 * expansion of the generating function
 *   G(x) = x²*(2x+1)² / ((1-8x)*(2x²-7x+1)*(4x²-6x+1))
 * resulting in a(2)=1, a(3)=25, a(4)=377, a(5)=4541, a(6)=48329.
 *
 * The implementation stores previously computed values in a static
 * cache and extends it on demand, so repeated calls with arbitrary
 * n are answered in O(1) after a linear precomputation.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* seq_t and idx_t are supplied by the build system; do NOT define them here. */

static seq_t *cache = NULL;     /* cache[i] corresponds to a(i+2) */
static idx_t  cache_size = 0;   /* number of terms currently in cache */
static idx_t  cache_cap   = 0;   /* allocated capacity */

/* Ensure the cache can hold at least 'needed' terms */
static inline void ensure_cache_size(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* allocation failed – abort with fatal error */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) with n ≥ 2; for n < 2 the value is 0. */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    /* indices in cache: cache[0] = a(2), cache[1] = a(3), … */
    idx_t target = n - 2;          /* 0‑based index of required term */
    ensure_cache_size(target + 1);

    /* extend the cache up to the required term */
    while (cache_size <= target) {
        idx_t cur_n = cache_size + 2;   /* current n to compute */

        /* RHS term from the numerator of G(x) non‑zero only for n=2,3,4 */
        seq_t rhs = 0;
        if (cur_n == 2)
            rhs = (seq_t)1;
        else if (cur_n == 3 || cur_n == 4)
            rhs = (seq_t)4;

        /* Positive contributions */
        seq_t pos = rhs;
        if (cur_n >= 3) /* 21*a(n-1) */
            pos += (seq_t)21 * cache[cur_n - 3];
        if (cur_n >= 5) /* 424*a(n-3) */
            pos += (seq_t)424 * cache[cur_n - 5];
        if (cur_n >= 7) /* 64*a(n-5) */
            pos += (seq_t)64 * cache[cur_n - 7];

        /* Negative contributions */
        seq_t neg = 0;
        if (cur_n >= 4) /* 152*a(n-2) */
            neg += (seq_t)152 * cache[cur_n - 4];
        if (cur_n >= 6) /* 328*a(n-4) */
            neg += (seq_t)328 * cache[cur_n - 6];

        /* Since the recurrence guarantees pos ≥ neg, unsigned subtraction is safe. */
        cache[cache_size] = pos - neg;
        cache_size++;
    }

    return cache[target];
}
