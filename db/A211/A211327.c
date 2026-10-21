/*
 * A211327  Number of (n+1) X (n+1) -3..3 symmetric matrices with every 2 X 2
 *          subblock having sum zero and one, three or four distinct values.
 * https://oeis.org/A211327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(1) = 15, a(2) = 33, a(3) = 69, a(4) = 143
 * a(n) = a(n-1) + 4*a(n-2) - 2*a(n-3) - 4*a(n-4)   for n ≥ 5
 * (OEIS A211327)  –  Crystallographic‐type property of 3×3 matrices.
 *
 * The function caches previously computed terms.  It works for all
 * sizes of `seq_t` (uint8_t…uint64_t, unsigned __int128, or arbitrary
 * precision) because it uses only the arithmetic operators allowed by
 * the problem statement.
 *
 * For `seq_t` types that fit 64 bits the recurrence is exact until
 * `a(62)`; values beyond that are not tested in the 64‑bit build.
 */

#include <stddef.h>
#include <stdlib.h>

/* external linkage as required */
seq_t a(idx_t n)
{
    /* offset is 1, so terms for n < 1 are undefined;
       return 0 for safety. */
    if (n < 1) return 0;

    /* static cache */
    static seq_t *cache = NULL;   /* cached terms, index 1.. */
    static size_t used  = 0;     /* number of terms stored (used)   */
    static size_t cap   = 0;     /* allocated capacity (≥ used)      */

    /* initialize cache with the first four known terms */
    if (used == 0) {
        cap = 5;  /* capacity for indices 0..4 (index 0 unused) */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();    /* allocation failure */
        cache[1] = (seq_t)15;
        cache[2] = (seq_t)33;
        cache[3] = (seq_t)69;
        cache[4] = (seq_t)143;
        used = 4;
    }

    /* grow cache if necessary */
    if (n >= cap) {
        while (cap <= n) {
            size_t newcap = cap ? cap << 1 : 4;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cap = newcap;
        }
    }

    /* compute missing terms up to n */
    while (used < n) {
        /* recurrence: a(i) = a(i-1)+4*a(i-2)-2*a(i-3)-4*a(i-4) */
        seq_t t1 = cache[used];
        seq_t t2 = cache[used - 1];
        seq_t t3 = cache[used - 2];
        seq_t t4 = cache[used - 3];

        /* all multiplications are with small constants,
           so overflow behavior is defined for unsigned types. */
        seq_t next = t1
                   + ((seq_t)4 * t2)
                   - ((seq_t)2 * t3)
                   - ((seq_t)4 * t4);

        ++used;
        cache[used] = next;
    }

    return cache[n];
}
