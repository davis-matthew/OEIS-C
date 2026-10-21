/*
 * A079991  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=3, r=3, I={-1,1}.
 * https://oeis.org/A079991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) +3*a(n-4) +a(n-6) -8*a(n-7) -a(n-8) -3*a(n-10)
 *            +2*a(n-13) +a(n-14), n ≥ 14 Initial values: a(0)=1, a(1)=1,
 *            a(2)=1, a(3)=2, a(4)=5, a(5)=13, a(6)=29, a(7)=58, a(8)=124,
 *            a(9)=280, a(10)=632, a(11)=1406, a(12)=3101, a(13)=6851 The
 *            recurrence yields all remaining terms
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Cache of computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;          /* current capacity of cache */

    /* Initial values (offset 0) */
    static const seq_t init[14] = {
        1, 1, 1, 2, 5, 13, 29, 58,
        124, 280, 632, 1406, 3101, 6851
    };

    /* Ensure cache is allocated and contains the first 14 values. */
    if (!cache || cap < 14) {
        size_t new_cap = (cap < 14) ? 14 : cap;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure – unused in tests */
        if (new_cap > cap) {
            /* zero new space (old part already set) */
            memset(tmp + cap, 0, (new_cap - cap) * sizeof(seq_t));
        }
        cache = tmp;
        cap   = new_cap;
        /* Copy initial values */
        memcpy(cache, init, 14 * sizeof(seq_t));
    }

    /* Expand cache if necessary to contain index n. */
    size_t needed = (size_t)n + 1;
    if (needed > cap) {
        size_t new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure – unused */
        /* zero new space */
        memset(tmp + cap, 0, (new_cap - cap) * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute values on demand using the recurrence. */
    for (size_t i = 14; i <= (size_t)n; ++i) {
        if (cache[i] == 0) {            /* not yet computed */
            seq_t val = 0;
            val += 2 * cache[i - 1];
            val += 3 * cache[i - 4];
            val +=       cache[i - 6];
            val -= 8 * cache[i - 7];
            val -=       cache[i - 8];
            val -= 3 * cache[i -10];
            val += 2 * cache[i -13];
            val +=       cache[i -14];
            cache[i] = val;
        }
    }

    return cache[(size_t)n];
}
