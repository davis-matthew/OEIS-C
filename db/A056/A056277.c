/*
 * A056277  Number of primitive (aperiodic) word structures of length n using
 *          a 6-ary alphabet.
 * https://oeis.org/A056277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} μ(d) · A056273(n/d) where A056273(k) = Σ_{j=1..6}
 *            S(k,j) (S = Stirling numbers of the second kind)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mul_small.h"   /* seq_t mul_small(seq_t x, unsigned int m); */

/* --------------------------------------------------------------------------
   Static state: arrays for A056273 (total word structures) and the
   required sequence A056277 (primitive structures).  All computations are
   performed with seq_t, which may be a fixed‑width unsigned integer
   or an arbitrary‑precision class.
   -------------------------------------------------------------------------- */

static seq_t *A_arr   = NULL;   /* A056273(n) for n ≥ 0 */
static seq_t *res_arr = NULL;   /* a(n) for n ≥ 0 */
static size_t prepared_max = 0;

/* --------------------------------------------------------------------------
   Compute A056273 up to N using the recurrence for Stirling numbers of
   the second kind:  S(n,k) = k·S(n-1,k) + S(n-1,k-1).
   The array S[0..6] holds the current row; S[0] is kept 0 for n > 0.
   -------------------------------------------------------------------------- */
static void compute_A_arr(size_t N)
{
    static seq_t S[7];   /* S[0..6] */

    /* Allocate space for A_arr */
    free(A_arr);
    A_arr = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!A_arr) exit(EXIT_FAILURE);

    /* Initialise first row: S(0,0) = 1, others 0 */
    S[0] = 1;
    for (int j = 1; j <= 6; ++j) S[j] = 0;

    for (size_t n = 1; n <= N; ++n) {
        /* Update S in descending order to use previous row values */
        for (int j = 6; j >= 1; --j)
            S[j] = mul_small(S[j], (unsigned int)j) + S[j - 1];
        S[0] = 0;   /* S(n,0) = 0 for n > 0 */

        /* Sum S(n,1)..S(n,6) to get A056273(n) */
        seq_t sum = 0;
        for (int j = 1; j <= 6; ++j)
            sum += S[j];
        A_arr[n] = sum;
    }
}

/* --------------------------------------------------------------------------
   Compute a(n) for all n ≤ N by the identity
       A(n) = Σ_{d|n} a(d)
   which yields
       a(n) = A(n) − Σ_{d|n, d<n} a(d)
   No negative intermediate values appear.
   -------------------------------------------------------------------------- */
static void compute_res_arr(size_t N)
{
    /* Allocate space for the result array */
    free(res_arr);
    res_arr = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!res_arr) exit(EXIT_FAILURE);

    for (size_t n = 1; n <= N; ++n) {
        seq_t sum = 0;
        /* Enumerate proper divisors of n using sqrt optimisation */
        for (size_t d = 1; d * d <= n; ++d) {
            if (n % d == 0) {
                /* divisor d */
                if (d < n)
                    sum += res_arr[d];

                /* paired divisor n/d */
                size_t e = n / d;
                if (e != d && e < n)
                    sum += res_arr[e];
            }
        }
        res_arr[n] = A_arr[n] - sum;
    }
}

/* --------------------------------------------------------------------------
   Recompute tables up to N if necessary.
   -------------------------------------------------------------------------- */
static void ensure_prepared(size_t N)
{
    if (N <= prepared_max)
        return;

    compute_A_arr(N);
    compute_res_arr(N);
    prepared_max = N;
}

/* --------------------------------------------------------------------------
   Public sequence function.
   -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* sequence is defined starting at offset 1 */

    size_t N = (size_t)n;
    ensure_prepared(N);
    return res_arr[n];
}
