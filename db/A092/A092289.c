/*
 * A092289  Number of triangles of partition numbers with the n-partition
 *          number as largest side.
 * https://oeis.org/A092289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(i,j) : 1 ≤ i ≤ j ≤ n and P(n) < P(i) + P(j)} where
 *            P(k) is the partition number (OEIS A000041)
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function relies on a dynamic cache of partition numbers and
   an O(n) two‑pointer scan to count the required pairs. */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are defined by the build system; do not redefine them here. */

/* Cache for partition numbers: P[0] = 1, P[1] = 1, ...  */
static seq_t *P = NULL;    /* array of partition numbers */
static idx_t  P_cap = 0;   /* current allocated size */
static idx_t  P_max = 0;   /* highest index computed so far */

/* Ensure that partition numbers up to n are available in the cache. */
static void ensure_partitions(idx_t n)
{
    if (n <= P_max) return;

    /* Grow the array to accommodate n */
    idx_t new_cap = P_cap;
    if (new_cap == 0) new_cap = 1;
    while (new_cap <= n) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(P, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);          /* abort on allocation failure */
    P = tmp;
    P_cap = new_cap;

    /* Initialise base case */
    if (P_max == 0) P[0] = (seq_t)1;

    /* Compute new partition values using Euler's pentagonal theorem. */
    for (idx_t k = P_max + 1; k <= n; ++k) {
        seq_t pos = 0;   /* sum of terms with + sign */
        seq_t neg = 0;   /* sum of terms with - sign */

        for (idx_t m = 1; ; ++m) {
            idx_t g1 = m * (3 * m - 1) / 2;
            if (g1 > k) break;
            seq_t term = P[k - g1];
            if (m & 1u) pos += term; else neg += term;

            idx_t g2 = m * (3 * m + 1) / 2;
            if (g2 > k) continue;
            term = P[k - g2];
            if (m & 1u) pos += term; else neg += term;
        }
        P[k] = pos - neg;
    }
    P_max = n;
}

/* Main function computing a(n). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined below offset */

    ensure_partitions(n);

    seq_t Pn = P[n];
    seq_t ans = 0;

    /* Two‑pointer technique: j is the minimal index that satisfies
       P[i] + P[j] > Pn.  As i increases, the minimal j never increases. */
    idx_t j = n;
    for (idx_t i = 1; i <= n; ++i) {
        if (j < i) j = i;  /* ensure j >= i */

        /* Move j leftwards while the inequality still holds. */
        while (j > i && (P[i] + P[j - 1]) > Pn) {
            --j;
        }

        /* If even the smallest j fails the strict inequality, skip. */
        if ((P[i] + P[j]) <= Pn) continue;

        /* All j' ≥ j satisfy the inequality: count them. */
        ans += (seq_t)(n - j + 1);
    }

    return ans;
}
