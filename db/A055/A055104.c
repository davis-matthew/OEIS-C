/*
 * A055104  Expansion of 1 + q/((1-q)*(1-q^2)) +
 *          q^2/((1-q)*(1-q^2)*(1-q^3)*(1-q^4)).
 * https://oeis.org/A055104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 (if n==0) , * a(n) = floor((n-1)/2)+1 + P4(n-2) (n>=1)
 *            * where P4(m) is the number of partitions of m using parts
 *            {1,2,3,4}
 * Range:     u8 n <= 29, u16 n <= 208, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes the partition numbers with parts 1–4
 * incrementally.  It works for all idx_t (uint64_t) values and any
 * seq_t integer type (uint8_t … unsigned __int128) provided by the build. */

#include <stdlib.h>

static seq_t *p4 = NULL;   /* array of P4 values */
static idx_t  p4_max = 0;  /* highest index for which p4 is computed */

/* Ensure that p4[0..needed] exists and contains the correct partition
 * counts for parts 1 through 4.  The array is recomputed entirely when
 * a larger value is required. */
static void ensure_p4(idx_t needed)
{
    if (p4 == NULL || needed > p4_max) {
        p4 = (seq_t *)realloc(p4, (size_t)(needed + 1) * sizeof(seq_t));
        /* initialize array to zero */
        for (idx_t i = 0; i <= needed; ++i) {
            p4[i] = 0;
        }
        p4[0] = 1;                         /* one way to partition 0 */
        /* classical coin‑change DP: unlimited parts 1,2,3,4 */
        for (idx_t part = 1; part <= 4; ++part) {
            for (idx_t s = part; s <= needed; ++s) {
                p4[s] += p4[s - part];
            }
        }
        p4_max = needed;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                    /* a(0) = 1 */

    /* coefficient from f1: floor((n-1)/2)+1 */
    seq_t term1 = ((seq_t)(n - 1)) / 2 + 1;

    if (n < 2)  /* only the f1 part contributes */
        return term1;

    /* add contribution from f2: partitions of (n-2) with parts 1–4 */
    ensure_p4(n - 2);
    return term1 + p4[n - 2];
}
