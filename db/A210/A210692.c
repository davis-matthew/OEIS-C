/*
 * A210692  Number of parts that are visible in one of the three views of the
 *          modular table of partitions with n regions mentioned in A210991.
 * https://oeis.org/A210692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(A000041(n)) = A026905(n) (a(n) is constant on * intervals
 *            [p(k), p(k+1)-1], where p(k) is the partition number). *
 *            A026905(k) satisfies b(0)=1, b(1)=1 and b(k)=b(k-1)+p(k) for
 *            k≥2. * Thus a(n) = b(k) where k is the largest integer with
 *            p(k)≤n
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>     /* for PRIu64 if needed */

static seq_t *p = NULL;          /* partition numbers p(k) */
static seq_t *b = NULL;          /* cumulative values A026905(k) */
static size_t p_len = 0;         /* number of computed partition numbers */

/* Compute the nth term a(n) (n≥1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* safety; sequence is defined for n≥1 */

    /* initialise with p[0]=1, b[0]=1. */
    if (p_len == 0) {
        p = (seq_t *)malloc(sizeof(seq_t));
        b = (seq_t *)malloc(sizeof(seq_t));
        p[0] = (seq_t)1;
        b[0] = (seq_t)1;
        p_len = 1;
    }

    /* extend the tables until the last partition number ≥ n. */
    while (p[p_len - 1] < (seq_t)n) {
        idx_t i = p_len;           /* next index to compute (0‑based) */
        /* compute p[i] using the pentagonal theorem */
        int64_t total = 0;         /* intermediate signed sum */
        for (idx_t j = 1; ; ++j) {
            idx_t g1 = j * (3 * j - 1) / 2;
            if (g1 > i) break;
            int64_t term = (int64_t)p[i - g1];
            total += (j & 1) ? term : -term;

            idx_t g2 = j * (3 * j + 1) / 2;
            if (g2 > i) continue;
            term = (int64_t)p[i - g2];
            total += (j & 1) ? term : -term;
        }
        seq_t pi = (seq_t)total;    /* value of p[i] */

        /* grow arrays */
        p = (seq_t *)realloc(p, sizeof(seq_t) * (i + 1));
        b = (seq_t *)realloc(b, sizeof(seq_t) * (i + 1));

        p[i] = pi;
        if (i == 1) {
            b[i] = (seq_t)1;        /* b[1] = 1 by definition */
        } else {
            b[i] = b[i - 1] + pi;   /* b[i] = b[i-1] + p[i] for i≥2 */
        }

        ++p_len;
    }

    /* find the largest k with p[k] ≤ n */
    idx_t k = p_len - 1;
    while (k > 0 && p[k] > (seq_t)n) --k;

    return b[k];
}
