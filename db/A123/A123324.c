/*
 * A123324  Number of integer-sided triangles with all sides <= n and sides
 *          relatively prime.
 * https://oeis.org/A123324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{c=1..n} sum_{d|c} μ(d) * T_all(c/d) where T_all(m) =
 *            (m even) ? (m/2)*(m/2+1) : ((m+1)/2)^2
 * Range:     u8 n <= 14, u16 n <= 96, u32 n <= 3955, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A123324: Number of integer‐sided triangles with all sides <= n
   and the three sides relatively prime (i.e. primitive triangles). */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;

    /* ---------- static storage ---------- */
    static int  *mu        = NULL;
    static long long *F      = NULL;
    static long long *prim   = NULL;
    static long long *cumA   = NULL;
    static size_t mu_sz   = 0;
    static size_t  f_sz   = 0;
    static size_t prim_sz = 0;
    static size_t cum_sz  = 0;

    size_t max_n = (size_t)n;

    /* ---------- resize arrays as needed ---------- */
    if (max_n + 1 > mu_sz) {
        size_t new_sz = max_n + 1;
        int  *new_mu = (int  *)realloc(mu,  sizeof(int)  * new_sz);
        if (!new_mu) return (seq_t)0;
        mu = new_mu;
        mu_sz = new_sz;
    }
    if (max_n + 1 > f_sz) {
        size_t new_sz = max_n + 1;
        long long *new_F = (long long *)realloc(F, sizeof(long long) * new_sz);
        if (!new_F) return (seq_t)0;
        F = new_F;
        f_sz = new_sz;
    }
    if (max_n + 1 > prim_sz) {
        size_t new_sz = max_n + 1;
        long long *new_prim = (long long *)realloc(prim, sizeof(long long) * new_sz);
        if (!new_prim) return (seq_t)0;
        prim = new_prim;
        prim_sz = new_sz;
    }
    if (max_n + 1 > cum_sz) {
        size_t new_sz = max_n + 1;
        long long *new_cum = (long long *)realloc(cumA, sizeof(long long) * new_sz);
        if (!new_cum) return (seq_t)0;
        cumA = new_cum;
        cum_sz = new_sz;
    }

    /* ---------- compute μ(k) up to max_n (linear sieve) ---------- */
    {
        size_t i, j, cnt;
        char *is_comp = (char *)calloc(max_n + 1, 1);
        int *primes   = (int *)malloc((max_n + 1) * sizeof(int));
        if (!is_comp || !primes) { free(is_comp); free(primes); return (seq_t)0; }
        mu[1] = 1;
        cnt = 0;
        for (i = 2; i <= max_n; ++i) {
            if (!is_comp[i]) {
                primes[cnt++] = (int)i;
                mu[i] = -1;
            }
            for (j = 0; j < cnt && i * (size_t)primes[j] <= max_n; ++j) {
                size_t ip = i * (size_t)primes[j];
                is_comp[ip] = 1;
                if (i % primes[j] == 0) {
                    mu[ip] = 0;
                    break;
                } else {
                    mu[ip] = -mu[i];
                }
            }
        }
        free(is_comp);
        free(primes);
    }

    /* ---------- compute T_all(m) for 1..max_n ---------- */
    {
        size_t m;
        for (m = 1; m <= max_n; ++m) {
            if (m & 1) { /* odd */
                long long a = (m + 1) / 2;
                F[m] = a * a;
            } else { /* even */
                long long a = m / 2;
                F[m] = a * (a + 1);
            }
        }
    }

    /* ---------- compute primitive counts via convolution ---------- */
    {
        size_t d, k, v;
        for (d = 1; d <= max_n; ++d) prim[d] = 0;
        for (d = 1; d <= max_n; ++d) {
            if (mu[d] == 0) continue;
            int sign = mu[d];
            for (k = 1, v = d; v <= max_n; ++k, v += d) {
                prim[v] += (long long)sign * F[k];
            }
        }
    }

    /* ---------- prefix sum of primitive counts ---------- */
    {
        size_t i;
        cumA[0] = 0;
        for (i = 1; i <= max_n; ++i) {
            cumA[i] = cumA[i - 1] + prim[i];
        }
    }

    return (seq_t)cumA[n];
}
