/*
 * A143724  Triangle read by rows, inverse Möbius transform of a diagonalized
 *          matrix of A116477.
 * https://oeis.org/A143724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A116477(k) if k divides r, else 0, where r is the row of
 *            n and k is the column index. A116477(k) = Σ_{d|k} μ(d) *
 *            S_{k/d}, with S_m = Σ_{i=1}^m τ(i) = Σ_{i=1}^m ⌊m/i⌋
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static idx_t  max_pre = 0;
static int   *mu      = NULL;
static seq_t *S       = NULL;
static seq_t *A       = NULL;

/* Prepare tables up to 'need' (inclusive). */
static void _ensure_pre(idx_t need)
{
    if (need <= max_pre) return;

    /* Free previous arrays. */
    free(mu);
    free(S);
    free(A);

    /* Allocate new arrays. */
    mu = (int*)malloc((need + 1) * sizeof(int));
    S  = (seq_t*)malloc((need + 1) * sizeof(seq_t));
    A  = (seq_t*)malloc((need + 1) * sizeof(seq_t));
    if (!mu || !S || !A) return;   /* allocation failure not expected */

    /* Linear sieve for μ (Möbius function). */
    idx_t *lp   = (idx_t*)calloc((need + 1), sizeof(idx_t));
    idx_t *pr   = (idx_t*)malloc((need + 1) * sizeof(idx_t));
    size_t pc   = 0;
    mu[1] = 1;
    for (idx_t i = 2; i <= need; ++i) {
        if (lp[i] == 0) {
            lp[i] = i;
            mu[i] = -1;
            pr[pc++] = i;
        }
        for (size_t j = 0; j < pc; ++j) {
            idx_t p = pr[j];
            idx_t x = i * p;
            if (x > need) break;
            lp[x] = p;
            if (i % p == 0) {
                mu[x] = 0;
                break;
            } else {
                mu[x] = -mu[i];
            }
        }
    }
    free(lp);
    free(pr);

    /* Divisor count τ via a basic sieve. */
    int *tau = (int*)calloc((need + 1), sizeof(int));
    for (idx_t i = 1; i <= need; ++i) {
        for (idx_t j = i; j <= need; j += i) {
            ++tau[j];
        }
    }

    /* Prefix sums S[i] = Σ_{t=1}^i τ(t). */
    S[0] = 0;
    for (idx_t i = 1; i <= need; ++i) {
        S[i] = S[i - 1] + (seq_t)tau[i];
    }
    free(tau);

    /* Möbius inversion to obtain A116477 values. */
    A[0] = 0;
    for (idx_t k = 1; k <= need; ++k) {
        long long val = 0;
        for (idx_t d = 1; d * d <= k; ++d) {
            if (k % d == 0) {
                int mu_d = mu[d];
                if (mu_d) val += (long long)mu_d * (long long)S[k / d];
                idx_t other = k / d;
                if (other != d) {
                    int mu_o = mu[other];
                    if (mu_o) val += (long long)mu_o * (long long)S[d];
                }
            }
        }
        A[k] = (seq_t)val;
    }

    max_pre = need;
}

/* External function required by the evaluation system. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* offset is 1; below is undefined */

    /* Find the row r such that r*(r+1)/2 >= n. */
    idx_t r = 1;
    while ((r * (r + 1) / 2) < n) ++r;

    _ensure_pre(r);                      /* precompute up to row r */

    idx_t prev = r * (r - 1) / 2;         /* elements before row r */
    idx_t k    = n - prev;                /* column index (1‑based) */

    if (r % k != 0) return (seq_t)0;     /* zero when k does not divide r */
    return A[k];                          /* otherwise the desired value */
}
