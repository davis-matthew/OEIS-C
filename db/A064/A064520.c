/*
 * A064520  a(n) = + 1 - 2 - 3 + 4 + 5 + 6 - 7 - 8 - 9 - 10 + 11 + 12 + 13 +
 *          14 + 15 - ... + (+-1)*n, where there is one plus, two minuses,
 *          three pluses, etc. (see A002024).
 * https://oeis.org/A064520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look, easy
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(A002024(k)+1)*k
 * Range:     i8 n <= 35, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   find_run
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* O(√n) time per call, memoised prefix sums for repeated calls */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/find_run.h"

seq_t a(idx_t n) {
    if (n == 0)          /* should not happen for offset 1 */
        return 0;

    /* Determine run number m and previous triangular number t_prev */
    idx_t m = find_run(n);                               /* run number (1‑based) */
    idx_t t_prev = (idx_t)(((unsigned __int128)(m - 1) * m) / 2); /* T_{m-1} */
    seq_t rem = (seq_t)(n - t_prev);           /* how many terms in current run */

    /* ---------- Compute sum of complete runs ---------- */
    /* We keep a static prefix array pref[0..k] where pref[j] = sum of first j runs */
    static seq_t *pref = NULL;
    static size_t pref_cap = 0;
    static size_t pref_len = 0;          /* number of elements stored ( +1 for pref[0]) */

    if (pref == NULL) {                        /* first call: allocate and init */
        pref_cap = 16;
        pref = (seq_t *)calloc(pref_cap, sizeof(seq_t));
        if (!pref) return 0;                   /* OOM */
        pref[0] = 0;
        pref_len = 1;
    }
    if (pref_cap < (size_t)m) {                /* enlarge storage if needed */
        size_t new_cap = pref_cap > 0 ? pref_cap : 16;
        while (new_cap < (size_t)m) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(pref, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                   /* OOM */
        pref = tmp;
        pref_cap = new_cap;
    }
    if (pref_len < (size_t)m) {                /* calculate missing prefix sums */
        for (size_t j = pref_len; j < (size_t)m; ++j) {
            idx_t jj = (idx_t)j;
            __int128 tmp = (__int128)jj * jj * jj + jj;    /* jj^3 + jj */
            seq_t term = (seq_t)(tmp / 2);                /* (jj^3 + jj)/2 */
            if (j & 1)
                pref[j] = pref[j - 1] + term;
            else
                pref[j] = pref[j - 1] - term;
        }
        pref_len = m;
    }
    seq_t sum_completed = pref[m - 1];

    /* ---------- Compute partial run contribution ---------- */
    seq_t start = (seq_t)(t_prev + 1);                     /* first term in run m */
    seq_t last  = start + rem - 1;                         /* last term in run m */
    seq_t sum_partial = (start + last) * rem / 2;          /* arithmetic series sum */

    /* Apply sign of run m (plus if m odd, minus if even) */
    if (m & 1)  /* odd run: plus sign */
        return sum_completed + sum_partial;
    else        /* even run: minus sign */
        return sum_completed - sum_partial;
}
