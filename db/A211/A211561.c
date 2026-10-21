/*
 * A211561  T(n,k) = number of nonnegative integer arrays of length n+k-1 with
 *          new values 0 upwards introduced in order, and containing the value
 *          k-1.
 * https://oeis.org/A211561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sum_{j = k - r}^{k} S(k, j), where k = m + 1, m =
 *            floor((√(8(n-1)+1)-1)/2), r = (n-1) - m(m+1)/2, and S(k, j) are
 *            Stirling numbers of the second kind
 * Range:     u8 n <= 24, u16 n <= 50, u32 n <= 129, u64 n <= 341,
 *            u128 n <= 932, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes S(k, j) for k up to 200 and uses
   a simple loop to find the triangular block containing n. */

#include <stdbool.h>
#include <stddef.h>

#define MAX_K 200 /* sufficient for n up to at least 10⁴ */

static void compute_stirling(seq_t S[MAX_K + 1][MAX_K + 1]) {
    /* Initialize table */
    for (size_t i = 0; i <= MAX_K; ++i) {
        for (size_t j = 0; j <= MAX_K; ++j) {
            S[i][j] = 0;
        }
    }
    S[0][0] = 1;
    for (size_t n = 1; n <= MAX_K; ++n) {
        for (size_t k = 1; k <= n; ++k) {
            seq_t term1 = S[n - 1][k - 1];
            seq_t term2 = S[n - 1][k];
            term2 = (seq_t)k * term2;   /* k * S(n-1, k) */
            S[n][k] = term1 + term2;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) {
        return 0; /* undefined for n=0, return 0 */
    }

    /* Static storage for Stirling numbers */
    static bool initialized = false;
    static seq_t S[MAX_K + 1][MAX_K + 1];
    if (!initialized) {
        compute_stirling(S);
        initialized = true;
    }

    /* Find m such that m(m+1)/2 <= n-1 < (m+1)(m+2)/2 */
    idx_t n1 = n - 1;
    idx_t m = 0;
    while (true) {
        idx_t next = (m + 1) * (m + 2) / 2;
        if (next > n1) {
            break;
        }
        ++m;
    }

    /* Compute r = (n-1) - m(m+1)/2 */
    idx_t Tm = m * (m + 1) / 2;
    idx_t r = n1 - Tm;

    idx_t k = m + 1;                     /* k <= MAX_K */
    idx_t j_low = k - r;                  /* inclusive lower bound */

    seq_t result = 0;
    for (idx_t j = j_low; j <= k; ++j) {
        result += S[k][j];
    }
    return result;
}
