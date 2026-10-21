/*
 * A209348  Number of 7-bead necklaces labeled with numbers -n..n allowing
 *          reversal, with sum zero with no three beads in a row equal.
 * https://oeis.org/A209348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 34, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A209348: Number of 7‑bead necklaces labeled with numbers –n..n
 * allowing reversal, with sum zero and no three equal consecutive beads.
 * Formula implemented: empirical recurrence (valid for n ≥ 19)
 *
 *   a(n) = 3*a(n-1) - 2*a(n-2) -  a(n-3) +  a(n-4) - 2*a(n-5)
 *          + 5*a(n-6) - 5*a(n-7) + 3*a(n-8) - 3*a(n-10) + 5*a(n-11)
 *          - 5*a(n-12) + 2*a(n-13) - a(n-14) + a(n-15)
 *          + 2*a(n-16) - 3*a(n-17) + a(n-18)
 *
 * Initial values (n = 1 … 18):
 *   a(1)=21, a(2)=514, a(3)=4029, a(4)=18646, a(5)=62853,
 *   a(6)=172610, a(7)=409199, a(8)=870122, a(9)=1699831,
 *   a(10)=3104474, a(11)=5365417, a(12)=8858142, a(13)=14068115,
 *   a(14)=21614144, a(15)=32266607, a(16)=46975088,
 *   a(17)=66888951, a(18)=93389664.
 *
 * The implementation memoises previously computed terms in a dynamic
 * array.  It works for any seq_t type that supports the usual arithmetic
 * operators and default construction.
 */

#include <stdlib.h> /* malloc, free */

static const seq_t BASE[18] = {
    21, 514, 4029, 18646, 62853,
    172610, 409199, 870122, 1699831, 3104474,
    5365417, 8858142, 14068115, 21614144, 32266607,
    46975088, 66888951, 93389664
};

static idx_t min_idx(idx_t a, idx_t b) { return (a < b) ? a : b; }

seq_t a(idx_t n)
{
    if (n == 0)          /* n<1 is out of range – undefined behaviour allowed */
        return 0;
    if (n <= 18)         /* return a base value directly */
        return BASE[n - 1];

    /* Memoisation cache */
    static seq_t *cache = NULL;   /* indices 0 .. last computed (cache[0] unused) */
    static idx_t last = 0;        /* largest n computed so far */

    /* allocate / grow cache if necessary */
    if (cache == NULL || last < n) {
        idx_t new_len = n + 1;              /* indices 0 .. n inclusive */
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_len);
        if (!new_cache)
            return 0; /* allocation failure – caller may handle 0 as error */

        /* copy existing values if any */
        if (cache) {
            for (idx_t i = 1; i <= last; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }

        /* initialise base values */
        for (idx_t i = 1; i <= 18; ++i)
            new_cache[i] = BASE[i - 1];

        cache = new_cache;
        last  = min_idx(new_len - 1, 18);
    }

    /* compute missing terms using the recurrence */
    idx_t start = (last < 19) ? 19 : last + 1;
    for (idx_t i = start; i <= n; ++i) {
        seq_t val = 0;
        val += 3 * cache[i - 1];
        val -= 2 * cache[i - 2];
        val -=     cache[i - 3];
        val +=     cache[i - 4];
        val -= 2 * cache[i - 5];
        val += 5 * cache[i - 6];
        val -= 5 * cache[i - 7];
        val += 3 * cache[i - 8];
        val -= 3 * cache[i -10];
        val += 5 * cache[i -11];
        val -= 5 * cache[i -12];
        val += 2 * cache[i -13];
        val -=     cache[i -14];
        val +=     cache[i -15];
        val += 2 * cache[i -16];
        val -= 3 * cache[i -17];
        val +=     cache[i -18];
        cache[i] = val;
    }
    last = n;
    return cache[n];
}
