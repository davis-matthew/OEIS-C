/*
 * A018810  Number of lines through exactly 3 points of an n X n grid of
 *          points.
 * https://oeis.org/A018810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/2 (f(n,4) - 2 f(n,3) + f(n,2)) * where f(n,k) = Σ ((n -
 *            |k*x|)(n - |k*y|)) * over all integer pairs (x,y) with
 *            gcd(x,y)=1, * |k*x| < n, |k*y| < n. * * The implementation uses
 *            the Möbius inversion identity * Σ_{gcd(x,y)=1} g(x)h(y) =
 *            Σ_{d=1}^{min(m,n)} μ(d) Σ_{x'} g(dx') Σ_{y'} h(dy') * w...
 * Range:     u8 n <= 8, u16 n <= 32, u32 n <= 567, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/*  Large enough to cover all arguments for n ≤ 10000 (k=2 gives M ≤ 4999) */
#define MAX_MU 20000
static int mu_arr[MAX_MU + 1];
static bool mu_ready = false;

/*  Compute the Möbius function up to MAX_MU once. */
static void init_mu(void)
{
    if (mu_ready) return;
    static char comp[MAX_MU + 1];
    static int primes[MAX_MU + 1];
    int pcnt = 0;

    mu_arr[1] = 1;
    for (int i = 2; i <= MAX_MU; ++i) {
        if (!comp[i]) {
            primes[pcnt++] = i;
            mu_arr[i] = -1;
        }
        for (int j = 0; j < pcnt; ++j) {
            int p = primes[j];
            long long prod = (long long)i * p;
            if (prod > MAX_MU) break;
            int ip = (int)prod;
            comp[ip] = 1;
            if (i % p == 0) {
                mu_arr[ip] = 0;
                break;
            } else {
                mu_arr[ip] = -mu_arr[i];
            }
        }
    }
    mu_ready = true;
}

/*  Compute f(n,k) for given n and k. */
static seq_t f_term(idx_t n, idx_t k)
{
    if (n <= k) return 0;

    idx_t M = (n - 1) / k;             /* maximum |x|,|y| allowed */
    seq_t n_sq = n;
    seq_t k_sq = k;

    /* contribution from axis directions (x=0 or y=0) */
    seq_t result = (seq_t)4 * n_sq * (n_sq - k_sq);

    for (idx_t d = 1; d <= M; ++d) {
        int mud = mu_arr[d];
        if (!mud) continue;

        idx_t L = M / d;
        seq_t L_sq = L;
        seq_t T = L_sq * n_sq - k_sq * (seq_t)d * (L_sq * (L_sq + 1) / 2);
        seq_t sq = T * T;               /* (T)^2 */

        if (mud == 1) {
            result += sq * 4;
        } else { /* mud == -1 */
            result -= sq * 4;
        }
    }

    return result;
}

/*  Main function required by the benchmark. */
seq_t a(idx_t n)
{
    init_mu();

    seq_t f2 = f_term(n, 2);
    seq_t f3 = f_term(n, 3);
    seq_t f4 = f_term(n, 4);

    /*  a = (f4 - 2*f3 + f2) / 2  -- implemented safely */
    seq_t sum = f2 + f4;
    seq_t two_f3 = f3 << 1;   /* multiply by 2 */
    seq_t diff = sum - two_f3;   /* guaranteed non‑negative */
    return diff >> 1;           /* divide by 2 */
}
