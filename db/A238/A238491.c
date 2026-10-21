/*
 * A238491  Number of partitions of n not containing 3*(number of parts) as a
 *          part.
 * https://oeis.org/A238491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p(n) – g(n‑3) where p(n) is the number of unrestricted
 *            partitions of n and g(m) is the number of partitions of m with
 *            all parts ≥ 4
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Handle the two small offset cases directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    /* Static state for memoisation */
    static seq_t *p_total = NULL;     /* total partitions */
    static seq_t *g_ge4   = NULL;     /* partitions with all parts ≥ 4 */
    static idx_t max_computed = 0;    /* largest n for which tables are built */

    /* Ensure tables are large enough */
    if (n > max_computed) {
        idx_t new_max = n;               /* need tables up to n */

        /* Allocate new tables */
        seq_t *new_p = (seq_t *)malloc(sizeof(seq_t) * (new_max + 1));
        seq_t *new_g = (seq_t *)malloc(sizeof(seq_t) * (new_max + 1));
        if (!new_p || !new_g) return (seq_t)0; /* allocation failed */

        /* Initialise to zero */
        for (idx_t i = 0; i <= new_max; ++i) {
            new_p[i] = 0;
            new_g[i] = 0;
        }
        new_p[0] = 1;   /* empty partition */
        new_g[0] = 1;   /* empty partition obtains empty part set ≥ 4 */

        /* Build unrestricted partition table */
        for (idx_t part = 1; part <= new_max; ++part) {
            for (idx_t sum = part; sum <= new_max; ++sum) {
                new_p[sum] += new_p[sum - part];
            }
        }

        /* Build partition table with all parts ≥ 4 */
        for (idx_t part = 4; part <= new_max; ++part) {
            for (idx_t sum = part; sum <= new_max; ++sum) {
                new_g[sum] += new_g[sum - part];
            }
        }

        /* Replace old tables */
        if (p_total) free(p_total);
        if (g_ge4)   free(g_ge4);
        p_total = new_p;
        g_ge4   = new_g;
        max_computed = new_max;
    }

    /* Compute a(n) = p(n) – g(n‑3) for n ≥ 3 */
    seq_t result = p_total[n];
    result -= g_ge4[n - 3];
    return result;
}
