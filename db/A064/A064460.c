/*
 * A064460  Number of distinct nonsquarefree entries in n-th row of Pascal's
 *          triangle.
 * https://oeis.org/A064460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 511, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of distinct nonsquarefree binomial coefficients in row n
 * of Pascal's triangle.
 *
 * For each k = 1 … ⌊n/2⌋ we iteratively update the prime‑exponent vector
 * for C(n,k+1) = C(n,k) * (n-k)/(k+1) and keep a running counter of
 * primes whose exponent has reached at least 2.  A binomial coefficient
 * is nonsquarefree iff that counter is > 0.  The function returns the
 * count of such coefficients in the row.
 *
 * Complexity:  O(n log n) time, O(n) memory (for the sieve).
 * Works for any seq_t/idx_t defined by the build.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* rows 0 and 1 are trivial */
    if (n <= 1) {
        return (seq_t)0;
    }

    /* ---------- 1. Sieve for smallest prime factors (spf) ---------- */
    size_t sz = (size_t)(n + 1);                     /* avoid overflow */
    idx_t *spf = (idx_t *)malloc(sz * sizeof(idx_t));
    int *prime_index = (int *)malloc(sz * sizeof(int));
    int *prime_list   = (int *)malloc(sz * sizeof(int));
    if (!spf || !prime_index || !prime_list) {
        /* allocation failed – return 0 as a fallback */
        free(spf); free(prime_index); free(prime_list);
        return (seq_t)0;
    }

    memset(spf, 0, sz * sizeof(idx_t));
    memset(prime_index, -1, sz * sizeof(int));

    int primes_cnt = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {            /* i is prime */
            spf[i] = i;
            prime_index[(int)i] = primes_cnt;
            prime_list[primes_cnt++] = (int)i;
            if (i * i <= n) {
                for (idx_t j = i * i; j <= n; j += i) {
                    if (spf[j] == 0) spf[j] = i;
                }
            }
        }
    }

    /* ---------- 2. Exponent array for primes ---------- */
    int *exp = (int *)calloc(primes_cnt, sizeof(int));
    if (!exp) {
        free(spf); free(prime_index); free(prime_list);
        return (seq_t)0;
    }
    int bad_count = 0;      /* number of primes with exponent ≥ 2 */
    seq_t counted = 0;      /* answer */

    /* ---------- 3. Iterate k from 1 to floor(n/2) ---------- */
    idx_t half = n / 2;
    for (idx_t k = 1; k <= half; ++k) {
        /* ---- add factor (n-k+1) ---- */
        idx_t t = n - k + 1;
        while (t > 1) {
            int p = (int)spf[t];
            int e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            int idxp = prime_index[p];
            int old = exp[idxp];
            int ne  = old + e;
            if (old < 2 && ne >= 2) ++bad_count;
            exp[idxp] = ne;
        }

        /* ---- subtract factor k ---- */
        t = k;
        while (t > 1) {
            int p = (int)spf[t];
            int e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            int idxp = prime_index[p];
            int old = exp[idxp];
            int ne  = old - e;
            if (old >= 2 && ne < 2) --bad_count;
            exp[idxp] = ne;
        }

        if (bad_count > 0) {
            ++counted;
        }
    }

    /* ---------- 4. Clean up ---------- */
    free(spf);
    free(prime_index);
    free(prime_list);
    free(exp);

    return counted;
}
