/*
 * A219237  Triangle of coefficients of the Gauss polynomials [n+4,4]_q
 *          (q-binomials).
 * https://oeis.org/A219237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) is the coefficient of q^k in the Gaussian * binomial
 *            [r+4,4]_q, where r is the largest integer with * 2 r² – r ≤ n,
 *            and k = n – (2 r² – r). This coefficient equals * the number of
 *            partitions of k into at most four parts each * ≤ r
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Determining r and k is O(√n), here performed with a short linear
 *  search as n ≤ 10 000 yields r ≤ 70.  The coefficient itself is
 *  computed with a small dynamic‑programming routine that runs in
 *  O(r·k) time (≤ ≈ 2·10⁴ elementary operations).  No static
 *  data structures are retained between calls.  All arithmetic uses
 *  the integer type seq_t supplied by the build. */

#include <stdint.h>
#include <stdbool.h>

/* Maximal r needed for n ≤ 10 000 (2r² – r ≈ 10 000) is 70.
 *  The maximal k for that r is 4r = 280, so allocating a buffer
 *  large enough for all possible rows is safe. */
#define MAX_R 70
#define MAX_K (4 * MAX_R)

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* ----- find row r and column k -------------------------------- */
    idx_t r = 0, sum = 0;
    /* start(r) = 2r² – r; we keep sum = start(r) as we iterate. */
    while (sum + (4 * r + 1) <= n) {
        sum += 4 * r + 1;
        ++r;
    }
    idx_t k = n - sum;          /* 0 ≤ k ≤ 4r */

    /* ----- DP for coefficient ------------------------------------- */
    seq_t dp[MAX_K + 1][5] = {0};   /* local array freshly zeroed */
    dp[0][0] = 1;

    for (idx_t part = 1; part <= r; ++part) {
        for (idx_t s = part; s <= 4 * r; ++s) {
            for (int p = 1; p <= 4; ++p) {
                dp[s][p] += dp[s - part][p - 1];
            }
        }
    }

    seq_t result = 0;
    for (int p = 0; p <= 4; ++p)
        result += dp[k][p];

    return result;
}
