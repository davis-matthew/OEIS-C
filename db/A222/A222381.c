/*
 * A222381  Sum of neighbor maps: number of n X 2 binary arrays indicating the
 *          locations of corresponding elements equal to the sum mod 3 of
 *          their horizontal and antidiagonal neighbors in a random 0..2 n X 2
 *          array.
 * https://oeis.org/A222381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence (valid for n ≥ 8): a(n) = 4*a(n‑1) +
 *            8*a(n‑3) - 32*a(n‑4) - 16*a(n‑6) + 64*a(n‑7) Initial values
 *            (offset 1): a(1)=2, a(2)=16, a(3)=64, a(4)=232, a(5)=1024,
 *            a(6)=4096, a(7)=16224 (the sequence grows quickly; terms after
 *            n≈30 exceed 64‑bit but the implementation works for any s...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 16, u64 n <= 31, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[1..cap] holds computed values */
    static idx_t cap   = 0;        /* allocated capacity (max index)      */
    static idx_t len   = 0;        /* largest index already computed      */

    /* 1. Allocate and initialise the base cache on first call.   */
    if (cache == NULL) {
        /* we need at least indices 1..7 to store the base values   */
        cap = 7;
        cache = (seq_t *)malloc((cap + 1) * sizeof(seq_t));
        if (!cache) abort();      /* allocation failure */

        cache[0] = 0;                /* unused (0‑based)          */
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)16;
        cache[3] = (seq_t)64;
        cache[4] = (seq_t)232;
        cache[5] = (seq_t)1024;
        cache[6] = (seq_t)4096;
        cache[7] = (seq_t)16224;
        len = 7;
    }

    /* 2. Grow storage if the requested index is beyond current cap.  */
    if (n > cap) {
        idx_t newcap = cap;
        while (newcap < n) {        /* double until sufficient */
            newcap <<= 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(newcap + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = newcap;
    }

    /* 3. Return cached value if already known. */
    if (n <= len) {
        return cache[n];
    }

    /* 4. Compute all missing terms up to n using the recurrence. */
    for (idx_t i = len + 1; i <= n; ++i) {
        /* the recurrence requires terms down to i‑7, those are already available */
        seq_t val = 4 * cache[i-1];
        val += 8 * cache[i-3];
        val -= 32 * cache[i-4];
        val -= 16 * cache[i-6];
        val += 64 * cache[i-7];
        cache[i] = val;
    }
    len = n;                     /* update largest computed index */
    return cache[n];
}
