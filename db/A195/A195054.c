/*
 * A195054  a(n) = p(n) - p(n-1) - p(n-2) + p(n-5), where p(n) = A000041(n).
 * https://oeis.org/A195054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = p(n) - p(n-1) - p(n-2) + p(n-5)
 * Range:     i8 n <= 22, i16 n <= 51, i32 n <= 141, i64 n <= 445,
 *            i128 n <= 1526, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where p(k) is the partition number and p(k)=0 for k<0. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static seq_t *partitions = NULL;   /* cache of partition numbers */
static size_t cap = 0;             /* allocated capacity */
static size_t len = 0;             /* number of computed entries (last index + 1) */

/* Ensure that partitions[0..n] are computed. */
static void ensure_partitions(idx_t n) {
    /* n is idx_t but we use size_t for array indices */
    if ((idx_t)len > 0 && n < (idx_t)len) return;   /* already available */

    size_t need = (size_t)n + 1;
    if (need > cap) {
        size_t new_cap = cap ? cap * 2 : 1;
        if (new_cap < need) new_cap = need;

#ifdef __cplusplus
        seq_t *new_arr = new seq_t[new_cap];
        /* default‑constructed for primitive types and big‑int types */
        for (size_t i = 0; i < len; ++i) new_arr[i] = partitions[i];
        delete[] partitions;
        partitions = new_arr;
#else
        seq_t *new_arr = realloc(partitions, new_cap * sizeof(seq_t));
        if (!new_arr) abort();   /* allocation failure */
        /* Zero new elements – safe for primitive integer types */
        memset(new_arr + cap, 0, (new_cap - cap) * sizeof(seq_t));
        partitions = new_arr;
#endif
        cap = new_cap;
    }

    /* Initialize p(0)=1 if necessary */
    if (len == 0) {
        partitions[0] = (seq_t)1;
        len = 1;
    }

    /* Compute partition numbers from len to n (inclusive) */
    for (size_t i = len; i <= (size_t)n; ++i) {
        seq_t sum = 0;
        for (size_t k = 1; ; ++k) {
            size_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i) break;
            seq_t term = partitions[i - pent1];
            if (k & 1) sum += term;
            else       sum -= term;

            size_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > i) continue;
            term = partitions[i - pent2];
            if (k & 1) sum += term;
            else       sum -= term;
        }
        partitions[i] = sum;
    }
    len = (size_t)n + 1;
}

/* ---- The requested function ---- */
seq_t a(idx_t n) {
    ensure_partitions(n);
    seq_t pn  = partitions[n];
    seq_t pn1 = (n >= 1) ? partitions[n - 1] : (seq_t)0;
    seq_t pn2 = (n >= 2) ? partitions[n - 2] : (seq_t)0;
    seq_t pn5 = (n >= 5) ? partitions[n - 5] : (seq_t)0;
    return pn - pn1 - pn2 + pn5;
}
