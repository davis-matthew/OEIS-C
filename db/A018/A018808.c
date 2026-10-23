/*
 * A018808  Number of lines through at least 2 points of an n X n grid of
 *          points.
 * https://oeis.org/A018808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = (f(n,1) – f(n,2)) / 2, where f(n,k) = Σ_{|x|<n,|y|<n,
 *            gcd(|x|,|y|)=k} (n-|x|)(n-|y|)
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 370, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*-------------------------------------------------------------
  Compute a(n) for the OEIS sequence A018808
  Number of lines through at least two points of an n×n grid.
-------------------------------------------------------------*/
#include <stdint.h>
#include <stddef.h>

#ifndef MAX_N
/* maximum n the function may be asked for */
#define MAX_N 10000
#endif

/* ---------- Möbius function up to MAX_N --------------------- */
static int mu[MAX_N + 1];
static unsigned char is_comp[MAX_N + 1];
static int primes[1000];
static int prime_cnt = 0;
static int mu_computed = 0;
static void compute_mu(void)
{
    if (mu_computed) return;
    mu[1] = 1;
    for (int i = 2; i <= MAX_N; ++i)
        is_comp[i] = 0;
    for (int i = 2; i <= MAX_N; ++i) {
        if (!is_comp[i]) {
            primes[prime_cnt++] = i;
            mu[i] = -1;
        }
        for (int j = 0; j < prime_cnt; ++j) {
            int p = primes[j];
            long long val = 1LL * i * p;
            if (val > MAX_N) break;
            is_comp[val] = 1;
            if (i % p == 0) {
                mu[val] = 0;
                break;
            } else {
                mu[val] = -mu[i];
            }
        }
    }
    mu_computed = 1;
}

/* ---------- Helper: compute S(n,k) --------------------------- */
static seq_t S_value(idx_t n, int k)
{
    /* sum_{d = k*m, m>=1, d <= n-1} μ(m) * ( M*n - d*M*(M+1)/2 )^2
       where M = floor((n-1)/d)  */
    signed __int128 sum = 0;
    idx_t max_m = (n - 1) / k;
    for (idx_t m = 1; m <= max_m; ++m) {
        int mu_val = mu[(int)m];
        if (mu_val == 0) continue;
        idx_t d = k * m;
        idx_t M = (n - 1) / d;
        if (M == 0) continue;
        signed __int128 t = (signed __int128)M * n
                         - (signed __int128)d * M * (M + 1) / 2;
        signed __int128 sq = t * t;
        sum += mu_val * sq;
    }
    return (seq_t)sum;
}

/* -------------------------------------------------------------
   Main function required by the build
------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    compute_mu();
    seq_t S1 = S_value(n, 1);
    seq_t S2 = S_value(n, 2);
    /* a(n) = 2*n + 2*(S1 - S2)   */
    seq_t two_n = (seq_t)2 * (seq_t)n;
    seq_t diff = (seq_t)(S1 - S2);
    seq_t result = two_n + (seq_t)2 * diff;
    return result;
}
