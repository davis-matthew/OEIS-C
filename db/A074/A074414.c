/*
 * A074414  Number of 5-ary Lyndon words of length n over Z_5 with trace 0 and
 *          subtrace 0.
 * https://oeis.org/A074414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of Lyndon words of length n over Z_5 with trace =
 *            0 and subtrace = 0. Computed as a(n) = N_lex(n) – Σ_{d|n, d<n}
 *            a(d), where N_lex(n) = (1/n) * Σ_{d|n} φ(n/d) * H(d,n), H(d,n) =
 *            5^d if (n/d)%5==0, S[d] otherwise, S[d] = number of ordinary
 *            length‑d strings over Z_5 with trace = subtr...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 26 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* required utilities */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Compute L_any[d] = number of ordinary Lyndon words of length d
   over alphabet size 5 (no constraints). */
static void compute_lyndon_any(idx_t n, const seq_t *pow5, seq_t *L_any)
{
    L_any[0] = 0;  /* unused */
    for (idx_t d = 1; d <= n; ++d) {
        seq_t total = pow5[d];
        for (idx_t e = 1; e * e <= d; ++e) {
            if (d % e == 0) {
                if (e < d) total -= (seq_t)e * L_any[e];
                idx_t f = d / e;
                if (f < d && f != e) total -= (seq_t)f * L_any[f];
            }
        }
        L_any[d] = total / d;
    }
}

/* Pre‑compute powers of 5 up to n. */
static void compute_powers(idx_t n, seq_t *pow5)
{
    pow5[0] = 1;
    for (idx_t i = 1; i <= n; ++i) pow5[i] = pow5[i - 1] * 5;
}

/* Compute S[d] = number of length‑d strings over Z_5 with trace
   and subtrace equal to 0 (mod 5).  DP over length. */
static void compute_strings(idx_t n, seq_t *S)
{
    /* 5×5 DP table: dp[sum][sum_sq] */
    seq_t (*dp)[5]  = mem_new(seq_t[5], 5);
    seq_t (*next)[5] = mem_new(seq_t[5], 5);
    if (!dp || !next) return;  /* abort silently on out‑of‑mem */

    dp[0][0] = 1;
    const int sq[5] = {0,1,4,4,1};

    for (idx_t len = 1; len <= n; ++len) {
        /* zero next */
        for (int i = 0; i < 5; ++i)
            for (int j = 0; j < 5; ++j)
                next[i][j] = 0;

        for (int sum = 0; sum < 5; ++sum)
            for (int sqr = 0; sqr < 5; ++sqr) {
                seq_t cur = dp[sum][sqr];
                if (!cur) continue;
                for (int a = 0; a < 5; ++a) {
                    int ns  = (sum + a) % 5;
                    int nsq = (sqr + sq[a]) % 5;
                    next[ns][nsq] += cur;
                }
            }

        /* copy next to dp */
        for (int i = 0; i < 5; ++i)
            for (int j = 0; j < 5; ++j)
                dp[i][j] = next[i][j];

        S[len] = dp[0][0];
    }

    mem_free(dp);
    mem_free(next);
}

/* Linear sieve for Euler's totient function φ up to n. */
static void compute_totient(idx_t n, uint64_t *phi)
{
    phi[1] = 1;
    for (idx_t i = 2; i <= n; ++i) phi[i] = i;
    for (idx_t i = 2; i <= n; ++i) {
        if (phi[i] == i) {
            for (idx_t j = i; j <= n; j += i) {
                phi[j] -= phi[j] / i;
            }
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    size_t N = (size_t)n + 1;
    seq_t *S   = mem_new(seq_t, N);
    seq_t *pow5 = mem_new(seq_t, N);
    seq_t *L_any= mem_new(seq_t, N);
    seq_t *a_val= mem_new(seq_t, N);
    uint64_t *phi= (uint64_t *)calloc(N, sizeof(uint64_t));

    if (!S || !pow5 || !L_any || !a_val || !phi) {
        /* memory failure – release any allocated memory */
        mem_free(S); mem_free(pow5); mem_free(L_any); mem_free(a_val); free(phi);
        return 0;
    }

    compute_totient(n, phi);
    compute_powers(n, pow5);
    compute_strings(n, S);
    compute_lyndon_any(n, pow5, L_any);

    /* Temporary array to hold divisors of current length. */
    idx_t divs[128];
    idx_t dcnt;

    for (idx_t l = 1; l <= n; ++l) {
        /* Gather divisors of l. */
        dcnt = 0;
        for (idx_t d = 1; d * d <= l; ++d) {
            if (l % d == 0) {
                divs[dcnt++] = d;
                if (d != l / d) divs[dcnt++] = l / d;
            }
        }

        /* Burnside: sum over divisors. */
        seq_t sum_fixes = 0;
        for (idx_t i = 0; i < dcnt; ++i) {
            idx_t d = divs[i];
            idx_t f = l / d;                /* factor = l/d */
            seq_t H;
            if (f % 5 == 0) H = pow5[d];
            else            H = S[d];
            sum_fixes += (seq_t)phi[f] * H;
        }
        seq_t N_lex = sum_fixes / l;        /* necklaces with property */

        /* Subtract necklaces from proper divisors. */
        seq_t sum_sub = 0;
        for (idx_t i = 0; i < dcnt; ++i) {
            idx_t d = divs[i];
            if (d == l) continue;
            idx_t f = l / d;
            seq_t Y;
            if (f % 5 == 0) Y = L_any[d];
            else            Y = a_val[d];
            sum_sub += Y;
        }

        a_val[l] = N_lex - sum_sub;
    }

    seq_t result = a_val[n];

    mem_free(S); mem_free(pow5); mem_free(L_any); mem_free(a_val); free(phi);
    return result;
}
