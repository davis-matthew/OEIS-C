/*
 * A097091  Number of partitions of n such that the least part occurs exactly
 *          three times.
 * https://oeis.org/A097091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -p(n) + 3*p(n+3) - p(n+4) - 2*p(n+5) + p(n+6)
 * Range:     u8 n <= 24, u16 n <= 56, u32 n <= 152, u64 n <= 466,
 *            u128 n <= 1566, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* p(n) is the partition function A000041. */

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

/* Static cache for partition numbers.
 * P[0] through P[maxComputed] are valid. */
static seq_t *P = NULL;          /* array p[0..maxComputed]   */
static idx_t maxComputed = 0;    /* largest n for which p[n] is known */

/* Compute partition numbers up to n_needed (inclusive). */
static void ensure_partitions(idx_t n_needed)
{
    if (n_needed <= maxComputed)
        return;

    /* Resize array if required */
    idx_t newSize = n_needed + 1;
    /* C++ requires a cast from void* to seq_t* */
    seq_t *newP = (seq_t *)realloc(P, newSize * sizeof(seq_t));
    if (!newP && newSize > 0)
        abort();   /* allocation error */
    P = newP;

    /* Initialise p[0] if it hasn't been done before. */
    if (maxComputed == 0) {
        /* Assume seq_t can be assigned from an integer literal. */
        P[0] = (seq_t)1;
    }

    /* Compute p[n] for n = maxComputed+1 .. n_needed. */
    for (idx_t n = maxComputed + 1; n <= n_needed; ++n) {
        seq_t pos = 0;
        seq_t neg = 0;

        /* Generalised pentagonal numbers via the recurrence. */
        for (idx_t k = 1; ; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > n) break;
            seq_t term = P[n - g1];
            if (k & 1)        /* odd k -> +1 */
                pos += term;
            else              /* even k -> -1 */
                neg += term;

            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 > n) continue;
            term = P[n - g2];
            if (k & 1)
                pos += term;
            else
                neg += term;
        }

        /* Partition number is the difference (guaranteed non‑negative). */
        P[n] = pos - neg;
    }

    maxComputed = n_needed;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined region, return 0 */

    /* Need partitions up to n+6. */
    ensure_partitions(n + 6);

    seq_t sumPos = 0;
    seq_t sumNeg = 0;

    /* -p(n) */
    sumNeg += P[n];

    /* +3*p(n+3) */
    sumPos += P[n + 3] * (seq_t)3;

    /* -p(n+4) */
    sumNeg += P[n + 4];

    /* -2*p(n+5) */
    sumNeg += P[n + 5] * (seq_t)2;

    /* +p(n+6) */
    sumPos += P[n + 6];

    return sumPos - sumNeg;
}
