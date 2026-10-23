/*
 * A024794  Number of 10's in all partitions of n.
 * https://oeis.org/A024794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+10) - a(n) = A000041(n); a(1) … a(9) = 0, a(10) = 1. The
 *            function computes the value for any n ≥ 1 using only integer
 *            arithmetic (no floating point). It is fully self‑contained and
 *            works for both C11 and C++17
 * Range:     u8 n <= 26, u16 n <= 52, u32 n <= 134, u64 n <= 413,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Helper for dynamic allocation that works in C and C++. */
static seq_t* allocate_seq(idx_t sz)
{
#ifdef __cplusplus
    return new seq_t[sz];
#else
    return (seq_t*)malloc((size_t)sz * sizeof(seq_t));
#endif
}

static void free_seq(seq_t* p)
{
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* out of range, but harmless */

    /* Handle the trivial small cases directly. */
    if (n <= 10) {
        return (seq_t)(n == 10 ? 1 : 0);
    }

    /* We need partition numbers up to (n‑10). */
    idx_t m = n - 10;
    seq_t *p = allocate_seq(m + 1);
    p[0] = (seq_t)1;                     /* p(0) = 1 */

    /* Pentagonal recurrence for the partition function p(k). */
    for (idx_t i = 1; i <= m; ++i) {
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1;; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > i) break;
            if (k & 1U) pos += p[i - g1]; else neg += p[i - g1];
            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 > i) continue;
            if (k & 1U) pos += p[i - g2]; else neg += p[i - g2];
        }
        p[i] = pos - neg;               /* pos ≥ neg, so safe */
    }

    /* Compute a(1)…a(n) using the recurrence a(i) = a(i‑10) + p(i‑10). */
    seq_t *a_arr = allocate_seq(n + 1);

    /* Base values up to 10. */
    for (idx_t i = 1; i <= 10; ++i) {
        a_arr[i] = (seq_t)(i == 10 ? 1 : 0);
    }

    for (idx_t i = 11; i <= n; ++i) {
        a_arr[i] = a_arr[i - 10] + p[i - 10];
    }

    seq_t result = a_arr[n];

    free_seq(p);
    free_seq(a_arr);
    return result;
}
