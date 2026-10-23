/*
 * A018591  Divisors of 684.
 * https://oeis.org/A018591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n‑th divisor of 684 (sorted in increasing order)
 * Range:     u8 n <= 13, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Comparison function for qsort, works for any unsigned seq_t type. */
static int seq_cmp(const void *p, const void *q) {
    const seq_t a = *(const seq_t*)p;
    const seq_t b = *(const seq_t*)q;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

seq_t a(idx_t n) {
    /* N must be ≥1 for a valid call; behaviour for n>18 is undefined
       (the sequence has only 18 terms). */
    if (n == 0) return 0;          /* safety for callers using 0‑based index */

    /* Pre‑computed powers of the prime factors of 684 = 2^2·3^2·19 */
    const seq_t pow2[3]  = {1, 2, 4};
    const seq_t pow3[3]  = {1, 3, 9};
    const seq_t pow19[2] = {1, 19};

    /* There are 3·3·2 = 18 divisors. */
    seq_t divisors[18];
    size_t count = 0;

    for (int e2 = 0; e2 < 3; ++e2)
        for (int e3 = 0; e3 < 3; ++e3)
            for (int e19 = 0; e19 < 2; ++e19)
                divisors[count++] = pow2[e2] * pow3[e3] * pow19[e19];

    /* Sort the divisors to satisfy the OEIS ordering. */
    qsort(divisors, count, sizeof(seq_t), seq_cmp);

    /* Return the requested term if within range, otherwise 0. */
    return (n <= count) ? divisors[n - 1] : (seq_t)0;
}
