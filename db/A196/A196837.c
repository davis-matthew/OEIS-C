/*
 * A196837  Coefficient table of numerator polynomials of o.g.f.s for partial
 *          sums of powers of positive integers.
 * https://oeis.org/A196837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) = T(r,c) where r = smallest integer with r(r+1)/2 ≥ n, c =
 *            n - (r-1)r/2 - 1 (0‑based column in row r), and T(r,c) = (r-c) *
 *            s(r+1, r+1-c) (signed Stirling numbers of the first kind)
 * Range:     i8 n <= 12, i16 n <= 32, i32 n <= 74, i64 n <= 205,
 *            i128 n <= 557, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define MAX_N 2000   /* sufficient for n up to ~2 million (r≈2000)           */

static seq_t S[MAX_N+1][MAX_N+1];
static bool s_init = false;

/* Pre‑compute signed Stirling numbers of the first kind up to MAX_N. */
static void init_stirling(void)
{
    if (s_init) return;
    s_init = true;

    S[0][0] = 1;
    for (idx_t n = 1; n <= MAX_N; ++n) {
        for (idx_t k = 0; k <= n; ++k) {
            if (k == 0 || k > n) {
                S[n][k] = 0;
            } else {
                /* s(n,k) = s(n-1,k-1) - (n-1)*s(n-1,k)  (signed Stirling) */
                S[n][k] = S[n-1][k-1] - (seq_t)(n-1) * S[n-1][k];
            }
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* 1‑based offset: a(1) is first term. */
    if (n == 0) return 0;   /* safety; not used for valid calls */

    /* Find the row r such that r(r+1)/2 ≥ n. */
    idx_t r = 0;
    idx_t sum = 0;
    while (sum < n) {
        ++r;
        sum += r;
    }

    /* Column index (0‑based) within row r. */
    idx_t prev_sum = sum - r;          /* sum of 1..(r-1) */
    idx_t c = n - prev_sum - 1;

    /* Ensure Stirling table is ready. */
    init_stirling();

    /* Compute the coefficient T(r,c) = (r-c) * s(r+1, r+1-c). */
    idx_t k = r + 1 - c;      /* second parameter for Stirling number */
    seq_t val = (seq_t)(r - c) * S[r+1][k];
    return val;
}
