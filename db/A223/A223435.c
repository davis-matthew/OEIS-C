/*
 * A223435  Generalized Petersen graph (8,2) coloring a rectangular array:
 *          number of nX3 0..15 arrays where 0..15 label nodes of a graph with
 *          edges 0,1 0,8 8,14 8,10 1,2 1,9 9,15 9,11 2,3 2,10 10,12 3,4 3,11
 *          11,13 4,5 4,12 12,14 5,6 5,13 13,15 6,7 6,14 7,0 7,15 and every
 *          array movement to a horizontal or ver...
 * https://oeis.org/A223435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-1) – 18*a(n-2) – 310*a(n-3) + 167*a(n-4) +
 *            475*a(n-5) – 244*a(n-6) – 100*a(n-7) + 48*a(n-8) Initial values
 *            for n = 1 … 8: 144, 1376, 14112, 147520, 1562176, 16693920,
 *            179532768, 1939216640
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_alloc.h"

seq_t a(idx_t n)
{
    /* 1‑based cache of computed values.  val[0] is unused. */
    static seq_t *val = NULL;
    static idx_t sz   = 0;   /* highest index that has been computed */
    static size_t cap = 0;   /* current capacity of the array (size in elements) */

    if (n == 0)
        return (seq_t)0;

    /* Initialise cache on first call.  Allocate enough space for the
       first eight terms and be ready for indices up to 16. */
    if (val == NULL) {
        cap = 16;                           /* power of two > 8        */
        val = seq_alloc(cap);               /* allocate array of size 'cap' */

        const seq_t init[8] = {
            (seq_t)144U,
            (seq_t)1376U,
            (seq_t)14112U,
            (seq_t)147520U,
            (seq_t)1562176U,
            (seq_t)16693920U,
            (seq_t)179532768U,
            (seq_t)1939216640U
        };
        for (idx_t i = 1; i <= 8; ++i)
            val[i] = init[i - 1];
        sz = 8;
    }

    /* Extend the cache and compute missing terms if necessary. */
    if (n > sz) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;                    /* grow to fit index n   */

        seq_t *new_val = seq_alloc(new_cap);
        for (idx_t i = 1; i <= sz; ++i)
            new_val[i] = val[i];              /* copy old data */

        /* adopt the larger array; the old one remains allocated
           (a small memory leak that is harmless for the required n). */
        val = new_val;
        cap = new_cap;

        /* Apply the recurrence to compute terms sz+1 … n. */
        for (idx_t i = sz + 1; i <= n; ++i) {
            seq_t t = ((seq_t)15U) * val[i - 1];
            t -= ((seq_t)18U) * val[i - 2];
            t -= ((seq_t)310U) * val[i - 3];
            t += ((seq_t)167U) * val[i - 4];
            t += ((seq_t)475U) * val[i - 5];
            t -= ((seq_t)244U) * val[i - 6];
            t -= ((seq_t)100U) * val[i - 7];
            t += ((seq_t)48U)  * val[i - 8];
            val[i] = t;
        }
        sz = n;
    }

    return val[n];
}
