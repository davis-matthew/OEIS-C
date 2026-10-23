/*
 * A193382  G.f. is the real part of the function C(x) that satisfies C(x) = 1
 *          + x/C(I*x).
 * https://oeis.org/A193382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   Let C_k be the Catalan numbers, C_0=1, C_{k} = C_{k-1} *
 *            2*(2*k-1)/(k+1). Define g_0 = 1 and for k ≥ 1, g_{4k} =
 *            (-1)^{k-1} * 2 * C_{k-1}, g_m = 0 otherwise. Let c_0 = 1, c_1 =
 *            2 and for n ≥ 2, c_n = 2 * c_{n-1} - 2 * c_{n-2} + g_n. Then the
 *            sequence is a_n = (c_n - c_{n-1} + δ_{n,0} + δ_{n,1}) /...
 * Range:     i8 n <= 16, i16 n <= 35, i32 n <= 51, i64 n <= 51, i128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* cached values of c[0..] */
    static seq_t *c = NULL;
    static idx_t cSize = 0;          /* number of stored c values (max index + 1) */

    /* cached Catalan numbers C[0..] */
    static seq_t *cat = NULL;
    static idx_t catSize = 0;        /* number of stored Catalan values (max index + 1) */

    /* ----------  ensure Catalan numbers up to floor(n/4) are computed ---------- */
    idx_t maxCatIndex = n / 4;       /* need C_{maxCatIndex-1} for g_{4*maxCatIndex} (if maxCatIndex>0) */
    if (catSize == 0) {
        /* initialise C_0 = 1 */
        cat = (seq_t*)malloc(sizeof(seq_t));
        if (!cat) return 0;         /* out of memory – caller will check */
        cat[0] = (seq_t)1;
        catSize = 1;
    }
    while (catSize <= maxCatIndex) {
        /* compute C_{catSize} based on C_{catSize-1} */
        idx_t k = catSize; /* new index to compute */
        seq_t prev = cat[catSize - 1];
        /* C_k = C_{k-1} * 2*(2*k-1) / (k+1)  (exact division) */
        seq_t num = prev * (seq_t)(2 * (2 * k - 1));
        seq_t den = (seq_t)(k + 1);
        seq_t next = num / den;
        cat = (seq_t*)realloc(cat, (catSize + 1) * sizeof(seq_t));
        cat[catSize] = next;
        ++catSize;
    }

    /* ----------  ensure c[0..n] are computed ---------- */
    if (n >= cSize) {
        /* initialise base cases if needed */
        if (cSize == 0) {
            c = (seq_t*)malloc(2 * sizeof(seq_t)); /* at least size 2 for c0 and c1 */
            if (!c) return 0;
            c[0] = (seq_t)1;                /* from recurrence */
            c[1] = (seq_t)2;                /* c1 = 2*c0 - 2*c-1 + g1 = 2*1 + 0 */
            cSize = 2;
        }
        /* extend the array to hold up to n */
        c = (seq_t*)realloc(c, (n + 1) * sizeof(seq_t));
        cSize = n + 1;
        /* compute new entries from the first missing index */
        for (idx_t i = (cSize > 2 ? cSize - 1 : 2); i <= n; ++i) {
            seq_t g = (seq_t)0;
            if (i == 0) {
                g = (seq_t)1;
            } else if (i % 4 == 0) {
                idx_t k = i / 4;                    /* k ≥ 1 */
                seq_t catPrev = cat[k - 1];
                /* sign = (-1)^(k-1) */
                seq_t sign = (k % 2 == 0) ? (seq_t)-1 : (seq_t)1;
                g = catPrev * (seq_t)2;
                if (sign < 0) g = -g;
            }
            seq_t prev1 = c[i - 1];
            seq_t prev2 = c[i - 2];
            c[i] = (seq_t)2 * prev1 - (seq_t)2 * prev2 + g;
        }
    }

    /* ----------  compute a(n) from c[n] and c[n-1] ---------- */
    seq_t cn     = c[n];
    seq_t cn_1   = (n == 0) ? (seq_t)0 : c[n - 1];
    seq_t delta  = (n == 0 || n == 1) ? (seq_t)1 : (seq_t)0;
    return (cn - cn_1 + delta) / (seq_t)2;
}
