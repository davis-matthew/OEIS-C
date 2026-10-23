/*
 * A122106  Numbers of the form (1+2n^2+3n^3)/(2n-1).
 * https://oeis.org/A122106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, sign
 * Formula:   a(n) = (1 + 2k^2 + 3k^3) / (2k-1) where k ranges over all
 *            integers for which the fraction is an integer. The domain is
 *            exactly the divisors of 15: k = (d+1)/2 with d | 15. The
 *            sequence is finite: [-1,0,3,6,11,20,62,111]. The function
 *            returns the nth term (1‑based) if it exists, otherwise 0
 * Range:     i8 n <= 8, i16 n <= 8, i32 n <= 8, i64 n <= 8, i128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static int cmp_seq(const void *a, const void *b) {
    const seq_t *va = (const seq_t *)a;
    const seq_t *vb = (const seq_t *)b;
    if (*va < *vb) return -1;
    if (*va > *vb) return 1;
    return 0;
}

static void init_terms(seq_t *terms, size_t *count) {
    /* Compute all k such that (2k-1) divides 15.  These are k = (d+1)/2
       for each divisor d of 15 (positive and negative). */
    const int absN = 15;
    int divs[16], dcnt = 0;
    for (int i = 1; i * i <= absN; ++i) {
        if (absN % i == 0) {
            divs[dcnt++] = i;
            if (i != absN / i) divs[dcnt++] = absN / i;
        }
    }
    size_t t = 0;
    for (int i = 0; i < dcnt; ++i) {
        for (int sgn = 1; sgn >= -1; sgn -= 2) {
            int d = sgn * divs[i];          /* divisor of 15 */
            long long k = (d + 1LL) / 2;    /* integer k */
            seq_t sk = (seq_t)k;
            seq_t val = ((seq_t)3 * sk * sk * sk
                         + (seq_t)2 * sk * sk
                         + (seq_t)1) /
                        (((seq_t)2 * sk) - (seq_t)1);
            terms[t++] = val;
        }
    }
    *count = t;
    qsort(terms, t, sizeof(seq_t), cmp_seq);
}

static seq_t *terms_global = NULL;
static size_t terms_count = 0;
static bool terms_ready = false;

static void ensure_terms(void) {
    if (!terms_ready) {
        terms_global = (seq_t *)malloc(sizeof(seq_t) * 8); /* exactly 8 terms */
        init_terms(terms_global, &terms_count);
        terms_ready = true;
    }
}

seq_t a(idx_t n) {
    ensure_terms();
    if (n == 0) return 0;
    size_t idx = (size_t)(n - 1);
    if (idx >= terms_count) return 0;
    return terms_global[idx];
}
