/*
 * A228177  Floor(n*(sqrt(6)+sqrt(5))).
 * https://oeis.org/A228177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n*(sqrt(6)+sqrt(5)) )
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 232, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 983 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a binary search with exact integer algebra.
 * It tests whether m <= n*alpha by comparing m^2 with 11*n^2 + 2*n^2*sqrt(30)
 * The inequalities avoid floating point and work for any integer type seq_t. */
#include <stdbool.h>

static inline bool leq_alpha(seq_t m, seq_t sn) {
    /* m <= n*alpha  ?
     * Let A = m^2 - 11*n^2.
     * If A < 0 the inequality holds.
     * If A >= 0 then require A^2 <= 120*n^4.        */
    seq_t n2 = sn * sn;            /* n^2  */
    seq_t m2 = m * m;              /* m^2  */
    seq_t A  = m2 - (seq_t)11 * n2; /* m^2 - 11*n^2 */
    if (A < 0) return true;
    /* now A >= 0 */
    seq_t A2 = A * A;              /* A^2 */
    seq_t rc = (seq_t)120 * n2 * n2;/* 120*n^4 */
    return A2 <= rc;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    seq_t sn = (seq_t)n;               /* n as seq_t */
    seq_t low  = 0;
    seq_t high = sn * 6 + 10;          /* high bound > n*alpha for all n */
    while (low <= high) {
        seq_t mid = (low + high) / 2;
        if (leq_alpha(mid, sn))
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high; /* high is the largest m with m <= n*alpha */
}
