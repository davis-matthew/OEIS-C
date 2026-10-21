/*
 * A224825  Expansion of psi(x) * psi(x^3)^2 in powers of x where psi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A224825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2500, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A224825
   Formula: a(n) = #{(i,j,k) ≥ 1 | i(i-1)/2 + 3j(j-1)/2 + 3k(k-1)/2 = n}
   (Count of triplets of positive integers whose triangular and
    "3‑triangular" exponents sum to n.)
   Off‑set 0: a(0)=1.  For n≥1 compute as described below.
   Complexity: O(√n · √n) per call, well under the stated limits.  */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* special case – the series starts with 1 at n = 0 */
    if (n == 0) return (seq_t)1;

    /* -------- compute the list of triangular numbers T(i)=i(i-1)/2 ≤ n -------- */
    idx_t tmax = 0;
    for (idx_t i = 1;; i++) {
        idx_t t = i * (i - 1) / 2;
        if (t > n) break;
        tmax++;
    }

    /* -------- compute the list of “3‑triangular” numbers F(j)=3j(j-1)/2 ≤ n -------- */
    idx_t fmax = 0;
    for (idx_t j = 1;; j++) {
        idx_t f = (idx_t)3 * j * (j - 1) / 2;
        if (f > n) break;
        fmax++;
    }

    /* allocate arrays to hold the values */
    idx_t *tri = (idx_t*)malloc(tmax * sizeof(idx_t));
    idx_t *farr = (idx_t*)malloc(fmax * sizeof(idx_t));
    if (!tri || !farr) {
        free(tri); free(farr);
        return 0;               /* memory allocation failure – return 0 */
    }

    /* fill the triangular array */
    idx_t m = 1;
    idx_t pos = 0;
    for (idx_t i = 1;; i++) {
        idx_t t = i * (i - 1) / 2;
        if (t > n) break;
        tri[pos++] = t;
    }

    /* fill the 3‑triangular array */
    pos = 0;
    for (idx_t j = 1;; j++) {
        idx_t f = (idx_t)3 * j * (j - 1) / 2;
        if (f > n) break;
        farr[pos++] = f;
    }

    /* boolean table for quick lookup of 3‑triangular values */
    bool *is_f = (bool*)malloc((n + 1) * sizeof(bool));
    if (!is_f) {
        free(tri); free(farr);
        return 0;
    }
    memset(is_f, 0, (n + 1) * sizeof(bool));
    for (idx_t k = 0; k < fmax; k++) {
        is_f[(size_t)farr[k]] = true;
    }

    /* count the triplets */
    seq_t result = 0;
    for (idx_t i = 0; i < tmax; i++) {
        idx_t Ti = tri[i];
        for (idx_t j = 0; j < fmax; j++) {
            idx_t Fj = farr[j];
            if (Ti + Fj > n) continue;      /* sum too large */
            idx_t need = n - Ti - Fj;       /* remaining to be supplied by k */
            if (need <= n && is_f[need])    /* does a k exist? */
                result++;
        }
    }

    /* clean up */
    free(tri);
    free(farr);
    free(is_f);

    return result;
}
