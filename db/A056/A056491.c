/*
 * A056491  Number of periodic palindromes using exactly five different
 *          symbols.
 * https://oeis.org/A056491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 60 * (S2(floor((n+1)/2),5) + S2(ceil((n+1)/2),5)) * where
 *            S2(m,k) is the Stirling subset number of the second kind. * The
 *            function precomputes S2(m, k) for all k ≤ 5 and 0 ≤ m ≤
 *            ceil((N+1)/2) * with N = 10000 (upper bound for requested
 *            indices). * It works for 64‑bit builds as well as for...
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 26, u64 n <= 53, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool _a056491_initialized = false;
#define MAX_N 10000ULL
/* ceil((MAX_N+1)/2) == 5001 for MAX_N=10000 */
#define MAX_M 5001ULL
/* Stirling numbers S2(m,k) for 0 ≤ k ≤ 5, 0 ≤ m ≤ MAX_M */
static seq_t _S2[6][MAX_M + 1];

static void _init_a056491(void)
{
    if (_a056491_initialized) return;
    /* Base cases */
    _S2[0][0] = 1;  /* S2(0,0) = 1 */
    for (idx_t k = 1; k <= 5; ++k) _S2[k][0] = 0;  /* S2(0,k>0) = 0 */
    /* Build table */
    for (idx_t m = 1; m <= MAX_M; ++m) {
        _S2[0][m] = 0;          /* S2(m,0) = 0 for m>0 */
        for (idx_t k = 1; k <= 5; ++k) {
            /* S2(m,k) = k*S2(m-1,k) + S2(m-1,k-1) */
            seq_t term1 = _S2[k][m-1];
            term1 = term1 * (seq_t)k;
            seq_t term2 = _S2[k-1][m-1];
            _S2[k][m] = term1 + term2;
        }
    }
    _a056491_initialized = true;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    _init_a056491();
    /* floor((n+1)/2) and ceil((n+1)/2) using integer arithmetic */
    idx_t m1 = (n + 1) / 2;   /* floor */
    idx_t m2 = (n + 2) / 2;   /* ceil */
    seq_t val = _S2[5][m1] + _S2[5][m2];
    return val * (seq_t)60;
}
