/*
 * A130703  a(n) = smallest k such that A000217(n+1) = A000217(n) +
 *          (A000217(n) mod k), or 0 if no such k exists.
 * https://oeis.org/A130703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that A000217(n+1) = A000217(n) +
 *            (A000217(n) mod k), or 0 if none exists
 * Range:     u8 n <= 11, u16 n <= 379, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n < 1, safe guard */
    seq_t Tn = (seq_t)n * ((seq_t)n + 1) / 2;   /* triangular number T_n */
    seq_t delta = (seq_t)(n + 1);                 /* difference Δ = n+1 */

    if (delta > Tn) return 0;            /* should not happen for n>=1 */
    seq_t D = Tn - delta;                /* D = T_n - (n+1) = (n+1)(n-2)/2 */

    /* No divisor can satisfy the condition if D == 0 (occurs only for n=2) or D < delta */
    if (D == 0 || D < delta) return 0;

    /* Search divisors of D; we need the smallest divisor > delta */
    idx_t limit = (idx_t)floor_sqrt(D);
    seq_t best = 0;                       /* 0 indicates no candidate found yet */

    for (idx_t d = 1; d <= limit; ++d)
    {
        if (D % d != 0) continue;

        seq_t dseq = (seq_t)d;
        seq_t partner = D / d;

        if (dseq > delta && (best == 0 || dseq < best))
            best = dseq;
        if (partner > delta && (best == 0 || partner < best))
            best = partner;
    }

    /* Any divisor we found is automatically <= D < Tn, so it satisfies k <= Tn */
    return best;
}
