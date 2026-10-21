/*
 * A061125  Number of degree-n permutations of order exactly 12.
 * https://oeis.org/A061125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = fAll[n] - f124[n] - f1236[n] + f12[n] where fS[n] =
 *            number of permutations of n whose cycle lengths belong to the
 *            set S. The recurrence for a set S is fS[0] = 1 fS[i] = Σ_{d∈S,
 *            d≤i} ((i-1)!/(i-d)!) * fS[i-d] The lcm of the allowed cycle
 *            lengths for the four sets are 2, 4, 6 and 12 respecti...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *f12    = NULL;
static seq_t *f124   = NULL;
static seq_t *f1236  = NULL;
static seq_t *fAll   = NULL;
static idx_t curMax  = 0;          /* largest n already computed */

static void ensure(idx_t upTo)
{
    if (upTo <= curMax) return;

    const size_t newSize = (size_t)upTo + 1;

    seq_t *tmp12   = (seq_t*)realloc(f12,    newSize * sizeof(seq_t));
    seq_t *tmp124  = (seq_t*)realloc(f124,   newSize * sizeof(seq_t));
    seq_t *tmp1236 = (seq_t*)realloc(f1236,  newSize * sizeof(seq_t));
    seq_t *tmpAll  = (seq_t*)realloc(fAll,   newSize * sizeof(seq_t));
    f12   = tmp12;
    f124  = tmp124;
    f1236 = tmp1236;
    fAll  = tmpAll;

    /* initialise index 0 (empty permutation) once */
    if (curMax == 0) {
        f12[0] = f124[0] = f1236[0] = fAll[0] = 1;
    }

    for (idx_t i = curMax + 1; i <= upTo; ++i) {
        seq_t val;

        /* set {1,2} */
        val = 0;
        const idx_t s12[] = {1, 2};
        for (int s = 0; s < 2; ++s) {
            idx_t d = s12[s];
            if (d > i) continue;
            seq_t ways = 1;
            for (idx_t t = i - d + 1; t <= i - 1; ++t)
                ways *= (seq_t)t;              /* (i-1)!/(i-d)!  */
            val += ways * f12[i - d];
        }
        f12[i] = val;

        /* set {1,2,4} */
        val = 0;
        const idx_t s124[] = {1, 2, 4};
        for (int s = 0; s < 3; ++s) {
            idx_t d = s124[s];
            if (d > i) continue;
            seq_t ways = 1;
            for (idx_t t = i - d + 1; t <= i - 1; ++t)
                ways *= (seq_t)t;
            val += ways * f124[i - d];
        }
        f124[i] = val;

        /* set {1,2,3,6} */
        val = 0;
        const idx_t s1236[] = {1, 2, 3, 6};
        for (int s = 0; s < 4; ++s) {
            idx_t d = s1236[s];
            if (d > i) continue;
            seq_t ways = 1;
            for (idx_t t = i - d + 1; t <= i - 1; ++t)
                ways *= (seq_t)t;
            val += ways * f1236[i - d];
        }
        f1236[i] = val;

        /* set {1,2,3,4,6,12} */
        val = 0;
        const idx_t sAll[] = {1, 2, 3, 4, 6, 12};
        for (int s = 0; s < 6; ++s) {
            idx_t d = sAll[s];
            if (d > i) continue;
            seq_t ways = 1;
            for (idx_t t = i - d + 1; t <= i - 1; ++t)
                ways *= (seq_t)t;
            val += ways * fAll[i - d];
        }
        fAll[i] = val;
    }

    curMax = upTo;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence defined for n ≥ 1 */
    ensure(n);
    seq_t res = fAll[n];
    res -= f124[n];
    res -= f1236[n];
    res += f12[n];
    return res;
}
