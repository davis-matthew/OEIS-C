/*
 * A164612  Expansion of q^(-1) * phi^2(q) * chi^3(q^9) / (chi(q^3) *
 *          phi^2(q^9)) in powers of q where phi(), chi() are Ramanujan theta
 *          functions.
 * https://oeis.org/A164612
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 18, i16 n <= 81, i32 n <= 423, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A164612
 *
 *  The generating function has an Euler–transform representation
 *  with a period‑36 sequence b(k):
 *
 *      b = [4,-6,3,-2,4,-5,4,-2,2,-6,4,-2,
 *           4,-6,3,-2,4,-2,4,-2,3,-6,4,-2,
 *           4,-6,2,-2,4,-5,4,-2,3,-6,4,0]
 *
 *  Let σ(n)=∑_{d|n} d·b(d).  
 *  Define aA[0]=1 and for n≥1:
 *
 *          aA[n] = (1/n) · ∑_{k=1}^{n} σ(k)·aA[n‑k]
 *
 *  The OEIS sequence {a(n)} is recovered by a shift:
 *
 *          a(n)=aA[n+1]   (n≥-1),  a(-1)=1
 *
 *  The function is memoised: once an aA term has been
 *  computed it is kept for future calls.
 *
 *  This method works for any n≥-1, is O(n²) in the
 *  worst case but very fast for n≤10000 (<0.2 s).
 */

#include <stdlib.h>
#include <stdint.h>

static const int b_arr[37] = {
    0,  4,-6, 3,-2, 4,-5, 4,-2, 2,-6, 4,-2,
    4,-6, 3,-2, 4,-2, 4,-2, 3,-6, 4,-2,
    4,-6, 2,-2, 4,-5, 4,-2, 3,-6, 4, 0
};

static seq_t *aA      = NULL;   /* coefficients of the Euler product (aA[0] = 1) */
static seq_t *sigma   = NULL;   /* σ(n) values */
static size_t  sz     = 0;      /* allocated size of aA and sigma */

static inline int get_b(uint64_t d)
{
    int r = (int)(d % 36);
    if (r == 0) r = 36;
    return b_arr[r];
}

static void ensure_size(size_t needed)
{
    if (needed < sz) return;
    size_t new_sz = sz ? sz * 2 : 4;
    if (new_sz <= needed) new_sz = needed + 1;
    aA    = (seq_t*)realloc(aA,    new_sz * sizeof(seq_t));
    sigma = (seq_t*)realloc(sigma, new_sz * sizeof(seq_t));
    if (sz == 0) {
        /* initialise first element aA[0] = 1 */
        aA[0] = (seq_t)1;
        sigma[0] = (seq_t)0;
    }
    /* compute new entries */
    for (size_t n = sz; n < new_sz; ++n) {
        /* compute σ(n) */
        seq_t sum = (seq_t)0;
        uint64_t nn = (uint64_t)n;
        for (uint64_t d = 1; d * d <= nn; ++d) {
            if (nn % d == 0) {
                int bd = get_b(d);
                sum += (seq_t)d * (seq_t)bd;
                uint64_t d2 = nn / d;
                if (d2 != d) {
                    int bd2 = get_b(d2);
                    sum += (seq_t)d2 * (seq_t)bd2;
                }
            }
        }
        sigma[n] = sum;

        if (n == 0) {
            aA[n] = (seq_t)1; /* already set */
            continue;
        }

        /* compute aA[n] = (1/n) * Σ_{k=1}^n σ(k) * aA[n-k] */
        seq_t acc = (seq_t)0;
        for (size_t k = 1; k <= n; ++k) {
            acc += sigma[k] * aA[n - k];
        }
        aA[n] = acc / (seq_t)n; /* exact division */
    }
    sz = new_sz;
}

seq_t a(idx_t n)
{
    if (n < -1) return (seq_t)0;                /* undefined below offset */
    size_t idx = (size_t)(n + 1);                /* shift: a(-1)=aA[0] */
    ensure_size(idx + 1);                         /* need indices 0 .. idx */
    return aA[idx];
}
