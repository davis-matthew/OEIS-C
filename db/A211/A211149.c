/*
 * A211149  Number of 2 X 2 nonsingular matrices having all terms in
 *          {-n,...,0,...,n}.
 * https://oeis.org/A211149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 2×2 nonsingular matrices with entries in
 *            {‑n,…,0,…,n} Let M = (2n+1)^2 be the number of 2‑vectors with
 *            coordinates in the range. Let total = M^2 = (2n+1)^4. Count of
 *            singular matrices = M + Σ_{m=1}^{n} K(m)·(4·t^2 + 2·t), where t
 *            = ⌊n/m⌋ and K(1) = 4 (vectors (±1,0), (0,±1), (±1,...
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static int *phi_cache = NULL;
static idx_t phi_limit = 0;

/* Compute Euler's totient values up to N using a simple sieve */
static void compute_phi(idx_t N)
{
    if (N <= phi_limit) return;
    if (phi_cache) free(phi_cache);
    phi_cache = (int*)malloc((size_t)(N + 1) * sizeof(int));
    for (idx_t i = 0; i <= N; ++i) phi_cache[i] = (int)i;
    if (N >= 1) phi_cache[1] = 1;
    for (idx_t i = 2; i <= N; ++i) {
        if (phi_cache[i] == (int)i) {          /* i is prime */
            for (idx_t j = i; j <= N; j += i) {
                phi_cache[j] -= phi_cache[j] / (int)i;
            }
        }
    }
    phi_limit = N;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined for offset 0 */
    /* precompute totients up to n */
    compute_phi(n);

    seq_t sn = (seq_t)n;
    seq_t two_n_plus_one = sn * 2 + 1;
    seq_t M = two_n_plus_one * two_n_plus_one;          /* (2n+1)^2 */
    seq_t total = M * M;                                /* (2n+1)^4 */

    /* Sum of contributions from primitive directions */
    seq_t sum = 0;
    for (idx_t m = 1; m <= n; ++m) {
        seq_t t = sn / m;            /* floor(n/m) */
        seq_t f = (seq_t)4 * t * t + (seq_t)2 * t;   /* 4t^2 + 2t */

        seq_t K;
        if (m == 1) {
            K = (seq_t)4;           /* (±1,0) and (0,±1) and (±1,±1) */
        } else {
            K = (seq_t)4 * (seq_t)phi_cache[m];
        }

        sum += K * f;
    }

    seq_t singular = M + sum;
    return total - singular;
}
