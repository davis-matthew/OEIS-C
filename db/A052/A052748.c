/*
 * A052748  Expansion of e.g.f.: -(log(1-x))^3.
 * https://oeis.org/A052748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6 * |s(n,3)| where s(n,3) are unsigned Stirling numbers
 *            of first kind. Recurrence (for unsigned counts):
 *            c(n,k)=c(n-1,k-1)+(n-1)*c(n-1,k), with c(0,0)=1, c(n,0)=0 for
 *            n>0, and we only need k=1,2,3
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Copyright holder: ChatGPT (2026) */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Static cache of a(n)=6*c(n,3) for n up to max_computed */
    static seq_t *c3 = NULL;          /* c(n,3) values */
    static size_t cap = 0;            /* allocated capacity of c3 */
    static idx_t max_n = 0;           /* largest n computed so far */

    /* previous Stirling values for k=1,2,3 at current n (before increment) */
    static seq_t prev_c1 = 0; /* c(n,1) */
    static seq_t prev_c2 = 0; /* c(n,2) */
    static seq_t prev_c3 = 0; /* c(n,3) */

    /* Ensure cache initialized for n=0 */
    if (cap == 0) {
        /* allocate small capacity, store c3[0]=0 */
        size_t init_cap = 4;
        c3 = (seq_t *)realloc(NULL, init_cap * sizeof(seq_t));
        if (!c3) return (seq_t)0; /* out of memory: return 0 (not expected) */
        cap = init_cap;
        c3[0] = (seq_t)0; /* a(0)=0 => c(0,3)=0 */
        max_n = 0;
        prev_c1 = (seq_t)0; /* c(0,1)=0 */
        prev_c2 = (seq_t)0; /* c(0,2)=0 */
        prev_c3 = (seq_t)0; /* c(0,3)=0 */
    }

    /* Expand cache if needed */
    if ((idx_t)cap <= n) {
        size_t needed = (size_t)(n + 1);
        size_t new_cap = cap ? cap : 4;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(c3, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* out of memory */
        c3 = tmp;
        cap = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        seq_t c1_new, c2_new, c3_new;

        /* c(k,1) */
        if (k == 1) {
            c1_new = (seq_t)1;
        } else {
            c1_new = (seq_t)(k - 1) * prev_c1;
        }

        /* c(k,2) */
        if (k == 1) {
            c2_new = (seq_t)0;
        } else if (k == 2) {
            c2_new = (seq_t)1;
        } else {
            c2_new = prev_c1 + (seq_t)(k - 1) * prev_c2;
        }

        /* c(k,3) */
        c3_new = prev_c2 + (seq_t)(k - 1) * prev_c3;

        /* store */
        c3[(size_t)k] = c3_new;

        /* update previous values for next iteration */
        prev_c1 = c1_new;
        prev_c2 = c2_new;
        prev_c3 = c3_new;
    }

    max_n = n;
    /* a(n) = 6 * c3[n] */
    return (seq_t)6 * c3[(size_t)n];
}
