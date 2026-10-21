/*
 * A209345  Number of 4-bead necklaces labeled with numbers -n..n allowing
 *          reversal, with sum zero with no three beads in a row equal.
 * https://oeis.org/A209345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) - 3*a(n-4) + 3*a(n-5) -
 *            a(n-6) (offset 1)
 * Range:     u8 n <= 6, u16 n <= 45, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used, but safe */

    static seq_t *cache = NULL;    /* cache[1] .. cache[n] */
    static idx_t cap   = 0;        /* allocated size */
    static idx_t upto   = 0;       /* highest index already computed */

    /* enlarge cache if needed */
    if (n > cap) {
        idx_t new_cap = cap ? cap * 2 : 16;
        while (new_cap < n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (new_cap + 1));
        cache = tmp;
        cap   = new_cap;
    }

    /* initialise first six terms if not yet done */
    if (upto < 6) {
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)15;
        cache[3] = (seq_t)35;
        cache[4] = (seq_t)72;
        cache[5] = (seq_t)128;
        cache[6] = (seq_t)205;
        upto = 6;
    }

    /* compute missing terms up to n (if any) */
    if (n > upto) {
        for (idx_t i = upto + 1; i <= n; ++i) {
            seq_t val = 3 * cache[i-1];
            val -= 3 * cache[i-2];
            val += 2 * cache[i-3];
            val -= 3 * cache[i-4];
            val += 3 * cache[i-5];
            val -= cache[i-6];
            cache[i] = val;
        }
        upto = n;
    }

    return cache[n];
}
