/*
 * A211558  Number of nonnegative integer arrays of length n+4 with new values
 *          0 upwards introduced in order, and containing the value 4.
 * https://oeis.org/A211558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=5..n+4} Stirling2(n+4,j)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define CAP 20030    /* ample limit for n <= 10000 */

static seq_t bell[CAP];          /* Bell numbers B[0..CAP-1]         */
static seq_t sumSCache[CAP];     /* partial sums S(.,1)+S(.,2)+S(.,3)+S(.,4) */
static seq_t S[5];               /* S(k) for current n (k=0..4)      */
static idx_t computedN = 0;      /* highest n for which data is known */

/* Compute a(n) for any n >= 1 */
seq_t a(idx_t n)
{
    idx_t N = n + 4;                 /* we need S(N,k) and Bell(N)       */

    /* initialise on first use */
    if (computedN == 0) {
        bell[0] = 1;
        sumSCache[0] = 0;
        S[0] = 1;
        for (int k = 1; k <= 4; ++k)
            S[k] = 0;
        computedN = 0;
    }

    /* extend precomputation up to N if necessary */
    if (N > computedN) {
        for (idx_t i = computedN; i < N; ++i) {
            /* compute S(i+1, k) for k=0..4 */
            seq_t nextS[5];
            nextS[0] = 0;
            for (int k = 1; k <= 4; ++k)
                nextS[k] = (seq_t)k * S[k] + S[k - 1];
            for (int k = 0; k <= 4; ++k)
                S[k] = nextS[k];

            /* compute Bell(i+1) using B[j] for j=0..i */
            idx_t m = i;
            seq_t binom = 1;      /* C(m,0) */
            seq_t sum   = 0;
            for (idx_t k = 0; k <= m; ++k) {
                sum += binom * bell[k];
                if (k < m)
                    binom = binom * (m - k) / (k + 1);
            }
            bell[i + 1] = sum;

            /* store partial Stirling sum for this n */
            sumSCache[i + 1] = S[1] + S[2] + S[3] + S[4];
        }
        computedN = N;
    }

    return bell[N] - sumSCache[N];
}
