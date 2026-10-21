/*
 * A094430  a(n) is the rightmost term of M^n * [1 0 0], where M is the 3 X 3
 *          matrix [0 1 0 / 0 0 1 / 7 -14 7].
 * https://oeis.org/A094430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 14*a(n-2) + 7*a(n-3); Initial values: a(1)=7,
 *            a(2)=49, a(3)=245. The function accepts n>=1. For n<1 it returns
 *            0 (unused)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 15, u64 n <= 31, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* maximum precomputed index; larger indices are computed on the fly. */
#define MAX_CACHE 20000

static seq_t cache[MAX_CACHE + 1] = {0};
static idx_t cache_filled = 0;   /* number of entries actually computed (>=0) */

/* helper to compute terms up to n, storing them in cache when possible */
static void compute_up_to(idx_t n)
{
    if (n <= cache_filled)            /* already computed */
        return;

    /* initialise first three terms if not already done */
    if (cache_filled < 3) {
        cache[1] = (seq_t)7U;
        cache[2] = (seq_t)49U;
        cache[3] = (seq_t)245U;
        cache_filled = 3;
    }

    if (n <= MAX_CACHE) {
        /* extend cache up to n using recurrence */
        for (idx_t i = cache_filled + 1; i <= n; ++i) {
            cache[i] = (seq_t)7 * cache[i-1]
                       - (seq_t)14 * cache[i-2]
                       + (seq_t)7 * cache[i-3];
        }
        cache_filled = n;
    } else {
        /* compute only the required values; use last three cache terms if available,
           otherwise initialise from scratch. */
        seq_t a_nm3, a_nm2, a_nm1;
        if (cache_filled >= 3) {
            a_nm3 = cache[cache_filled-2];
            a_nm2 = cache[cache_filled-1];
            a_nm1 = cache[cache_filled];
        } else {
            /* start from the initial values */
            a_nm3 = (idx_t)0U;          /* unused for i <= 3 */
            a_nm2 = (idx_t)0U;
            a_nm1 = (seq_t)7U;
            if (cache_filled >= 1) a_nm1 = cache[1];
            if (cache_filled >= 2) a_nm2 = cache[2];
            if (cache_filled >= 3) a_nm3 = cache[3];
        }

        for (idx_t i = (cache_filled > 3 ? cache_filled+1 : 4); i <= n; ++i) {
            seq_t a_n = (seq_t)7 * a_nm1
                        - (seq_t)14 * a_nm2
                        + (seq_t)7 * a_nm3;
            a_nm3 = a_nm2;
            a_nm2 = a_nm1;
            a_nm1 = a_n;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1U)
        return (seq_t)0U;   /* offset is 1; caller may pass 0 */

    compute_up_to(n);

    /* if n was beyond MAX_CACHE we didn't store it; compute on the fly */
    if (n <= cache_filled)   /* was stored */
        return cache[n];

    /* compute transiently */
    seq_t a_nm3, a_nm2, a_nm1;
    if (cache_filled >= 3) {
        a_nm3 = cache[cache_filled-2];
        a_nm2 = cache[cache_filled-1];
        a_nm1 = cache[cache_filled];
    } else {
        a_nm3 = (idx_t)0U;
        a_nm2 = cache[1];
        a_nm1 = (cache_filled >= 2) ? cache[2] : (seq_t)49U;
    }

    for (idx_t i = (cache_filled > 3 ? cache_filled+1 : 4); i <= n; ++i) {
        seq_t a_n = (seq_t)7 * a_nm1
                    - (seq_t)14 * a_nm2
                    + (seq_t)7 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
