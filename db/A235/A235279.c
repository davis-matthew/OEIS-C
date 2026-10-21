/*
 * A235279  Number of (n+1) X (9+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 5
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A235279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 21, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A235279 implemented via its 9‑term linear recurrence.
 * Recurrence: a(n)=a(n-1)+15*a(n-2)-15*a(n-3)-80*a(n-4)
 *              +80*a(n-5)+180*a(n-6)-180*a(n-7)-144*a(n-8)+144*a(n-9)
 * Initial values (for n=1…9):
 * 35200, 41500, 48328, 62800, 79480,
 * 115480, 160888, 260680, 401560
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of computed values. cache[0] is unused. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;     /* number of allocated slots */
    static idx_t max_computed = 0;   /* largest index that has been filled */

    if (n == 0)
        return 0;                    /* base case outside the OEIS range */

    /* ---------- Ensure storage for indices 0 … n ---------- */
    if (cache_size <= n) {
        /* Make sure we have at least 10 slots in case n < 9. */
        idx_t new_size = (n + 1 > (idx_t)10) ? (n + 1) : (idx_t)10;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!tmp)
            abort(); /* allocation failed */
        /* Zero the newly allocated part. */
        for (size_t i = (size_t)cache_size; i < (size_t)new_size; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_size = new_size;
    }

    /* ---------- Initialize the first 9 terms if not already done ---------- */
    if (max_computed < 9) {
        /* Initial values are stored in a 1‑based constant array.            */
        static const seq_t init[10] = {
            0,          /* index 0 unused */
            35200,      /* a(1) */
            41500,      /* a(2) */
            48328,      /* a(3) */
            62800,      /* a(4) */
            79480,      /* a(5) */
            115480,     /* a(6) */
            160888,     /* a(7) */
            260680,     /* a(8) */
            401560      /* a(9) */
        };
        for (idx_t i = 1; i <= 9; ++i)
            cache[i] = init[i];
        max_computed = 9;
    }

    /* ---------- Compute terms up to n using the recurrence ---------- */
    if (max_computed < n) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            /* The recurrence uses only indices less than i,
             * so all required values are already computed.          */
            seq_t val = cache[i - 1];
            val += (seq_t)15 * cache[i - 2];
            val -= (seq_t)15 * cache[i - 3];
            val -= (seq_t)80 * cache[i - 4];
            val += (seq_t)80 * cache[i - 5];
            val += (seq_t)180 * cache[i - 6];
            val -= (seq_t)180 * cache[i - 7];
            val -= (seq_t)144 * cache[i - 8];
            val += (seq_t)144 * cache[i - 9];
            cache[i] = val;
        }
        max_computed = n;
    }

    return cache[n];
}
