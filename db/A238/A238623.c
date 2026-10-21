/*
 * A238623  Number of partitions of n such that neither floor(n/2) nor
 *          ceiling(n/2) is a part.
 * https://oeis.org/A238623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p(n) - p(n - ⌊n/2⌋) - p(n - ⌈n/2⌉) + p(0) (n odd) a(n) =
 *            p(n) - p(n/2) (n even) where p(k) is the number of unrestricted
 *            integer partitions of k
 * Range:     u8 n <= 17, u16 n <= 43, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for the partition numbers p[0..max] */
static seq_t *P = NULL;   /* array of partitions */
static idx_t Psize = 0;   /* allocated size of P */

/* Compute p(k) for all 0 ≤ k ≤ limit (inclusive). */
static void recompute_partitions(idx_t limit)
{
    idx_t needed = limit + 1;
    if (P == NULL || needed > Psize) {
        seq_t *newP = (seq_t *)realloc(P, needed * sizeof(seq_t));
        if (!newP) return;           /* allocation failure – caller will get zero */
        P = newP;
        Psize = needed;
    }

    /* Zero the array (safe: seq_t handles 0 assignment). */
    for (idx_t i = 0; i < needed; ++i) P[i] = 0;
    P[0] = 1;                       /* one partition of zero */

    /* Classic dynamic programming: for each part size i, add it to all larger sums. */
    for (idx_t i = 1; i <= limit; ++i) {
        for (idx_t j = i; j <= limit; ++j) {
            P[j] = P[j] + P[j - i];
        }
    }
}

/* Sequence function asked for. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, but safe */

    recompute_partitions(n);

    seq_t total = P[n];
    if (n & 1) {                   /* n is odd */
        idx_t m = n / 2;            /* floor(n/2) */
        idx_t c = n - m;            /* ceil(n/2)  */
        seq_t res = total - P[n - m] - P[n - c] + P[0];
        return res;
    } else {                       /* n is even */
        idx_t half = n / 2;
        seq_t res = total - P[n - half];
        return res;
    }
}
