/*
 * A165529  Number of permutations of length n which avoid the patterns 4312
 *          and 2143.
 * https://oeis.org/A165529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of permutations of length n avoiding 4312 and 2143.
 *            Computed via the linear recurrence derived from the rational
 *            g.f. Recurrence (for n ≥ 10): a(n) = 17·a(n-1) -124·a(n-2)
 *            +507·a(n-3) -1275·a(n-4) +2040·a(n-5) -2083·a(n-6) +1331·a(n-7)
 *            -508·a(n-8) +105·a(n-9) -9·a(n-10) Initial values: a(0...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, free */

seq_t a(idx_t n) {
    /* initial values */
    static const seq_t init[10] = {
        (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)6,
        (seq_t)22, (seq_t)86, (seq_t)337, (seq_t)1295,
        (seq_t)4854, (seq_t)17760
    };

    if (n <= 9) {
        return init[(size_t)n];
    }

    /* allocate array for sequence values up to n */
    seq_t *seq = (seq_t*)malloc(((size_t)n + 1ULL) * sizeof(seq_t));
    if (!seq) {  /* fallback: return 0 on allocation failure */
        return (seq_t)0;
    }

    /* fill initial segment */
    for (size_t i = 0; i <= 9; ++i) {
        seq[i] = init[i];
    }

    /* compute remaining terms */
    for (size_t i = 10; i <= (size_t)n; ++i) {
        /* compute positive part */
        seq_t pos = (seq_t)0;
        pos += seq[i - 1] * (seq_t)17;     /* 17*a(n-1) */
        pos += seq[i - 3] * (seq_t)507;    /* 507*a(n-3) */
        pos += seq[i - 5] * (seq_t)2040;   /* 2040*a(n-5) */
        pos += seq[i - 7] * (seq_t)1331;   /* 1331*a(n-7) */
        pos += seq[i - 9] * (seq_t)105;    /* 105*a(n-9) */

        /* compute negative part */
        seq_t neg = (seq_t)0;
        neg += seq[i - 2] * (seq_t)124;    /* 124*a(n-2) */
        neg += seq[i - 4] * (seq_t)1275;   /* 1275*a(n-4) */
        neg += seq[i - 6] * (seq_t)2083;   /* 2083*a(n-6) */
        neg += seq[i - 8] * (seq_t)508;    /* 508*a(n-8) */
        neg += seq[i - 10] * (seq_t)9;     /* 9*a(n-10) */

        seq[i] = pos - neg;   /* pos ≥ neg for all n */
    }

    seq_t result = seq[(size_t)n];
    free(seq);
    return result;
}
