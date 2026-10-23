/*
 * A008424  Theta series of {D_9}* lattice.
 * https://oeis.org/A008424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of q^n in (θ₂(q⁴))⁹ + (θ₃(q⁴))⁹, where θ₂(q)
 *            = 2∑_{k≥0} q^{(k+½)²} and θ₃(q) = 1 + 2∑_{k>0} q^{k²}
 * Range:     u8 n <= 8, u16 n <= 40, u32 n <= 864, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

typedef struct {
    size_t exp;
    seq_t weight;
} Term;

/* Convolution that raises a single-variable generating function to the power `repeats`. */
static void convolution_pow(seq_t *res, size_t maxn, const Term *terms,
                            size_t termcnt, int repeats)
{
    seq_t *cur = (seq_t*)calloc(maxn + 1, sizeof(seq_t));
    cur[0] = 1;                           /* start with the constant term */

    for (int r = 0; r < repeats; ++r) {
        seq_t *next = (seq_t*)calloc(maxn + 1, sizeof(seq_t));
        for (size_t i = 0; i <= maxn; ++i) {
            seq_t curv = cur[i];
            if (!curv) continue;          /* skip zero entries */
            for (size_t t = 0; t < termcnt; ++t) {
                size_t e = terms[t].exp;
                size_t ni = i + e;
                if (ni > maxn) break;      /* terms are sorted by increasing exp */
                next[ni] += curv * terms[t].weight;
            }
        }
        free(cur);
        cur = next;
    }

    memcpy(res, cur, (maxn + 1) * sizeof(seq_t));
    free(cur);
}

seq_t a(idx_t n)
{
    size_t maxN = (size_t)n;

    /* Arrays to hold the two generating functions up to maxN. */
    seq_t *A = (seq_t*)calloc(maxN + 1, sizeof(seq_t));
    seq_t *B = (seq_t*)calloc(maxN + 1, sizeof(seq_t));

    /* Build terms for (θ₃(q⁴))⁹ : exponents 4·m², weight 1 for m=0, 2 otherwise. */
    Term terms3[128];
    size_t cnt3 = 0;
    for (size_t m = 0; ; ++m) {
        size_t e = 4 * m * m;
        if (e > maxN) break;
        terms3[cnt3].exp = e;
        terms3[cnt3].weight = (m == 0) ? (seq_t)1 : (seq_t)2;
        ++cnt3;
    }
    convolution_pow(A, maxN, terms3, cnt3, 9);

    /* Build terms for (θ₂(q⁴))⁹ : exponents (2·m+1)², weight 2. */
    Term terms2[128];
    size_t cnt2 = 0;
    for (size_t m = 0; ; ++m) {
        size_t e = (2 * m + 1) * (2 * m + 1);
        if (e > maxN) break;
        terms2[cnt2].exp = e;
        terms2[cnt2].weight = (seq_t)2;
        ++cnt2;
    }
    convolution_pow(B, maxN, terms2, cnt2, 9);

    seq_t result = A[n] + B[n];

    free(A);
    free(B);
    return result;
}
