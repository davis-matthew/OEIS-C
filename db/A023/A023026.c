/*
 * A023026  Number of partitions of n into 6 unordered relatively prime parts.
 * https://oeis.org/A023026
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k|n} μ(k) * p₆(n/k) where p₆(m) = number of partitions
 *            of m into exactly 6 unordered parts. p₆ can be computed
 *            recursively via p_r(0) = 1 for r = 0, 0 otherwise p_r(n) = 0 for
 *            n < r p_r(n) = p_r(n-r) + p_{r-1}(n-1) for n ≥ r, r ≥ 1 The
 *            storage below is exact for all n ≤ 10000; the algor...
 * Range:     u8 n <= 25, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifndef NMAX
#define NMAX 10000          /* maximum n handled accurately in the limit */
#endif

/* static tables used for memoised computation */
static bool _initialised = false;
static int _mu[NMAX + 1];
static seq_t _p[7][NMAX + 1];

/* initialise μ and p tables once */
static void _init(void)
{
    if (_initialised)
        return;
    _initialised = true;

    /* --- compute Möbius function up to NMAX --- */
    int primes[1229], pcnt = 0;   /* π(10000)=1229 */
    int spf[NMAX + 1] = {0};
    _mu[1] = 1;
    for (int i = 2; i <= NMAX; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes[pcnt++] = i;
            _mu[i] = -1;
        }
        for (int j = 0; j < pcnt && primes[j] <= spf[i] && (long long)i * primes[j] <= NMAX; ++j) {
            int v = i * primes[j];
            spf[v] = primes[j];
            if (i % primes[j] == 0) {
                _mu[v] = 0;
            } else {
                _mu[v] = -_mu[i];
            }
        }
    }

    /* --- compute p_r(n) for r = 0..6, n = 0..NMAX --- */
    for (int r = 0; r <= 6; ++r) {
        for (int n = 0; n <= NMAX; ++n) {
            if (r == 0) {
                _p[0][n] = (n == 0 ? (seq_t)1 : (seq_t)0);
            } else if (n < r) {
                _p[r][n] = (seq_t)0;
            } else if (r == 1) {
                _p[1][n] = (seq_t)1;          /* single part */
            } else {
                seq_t val = _p[r][n - r];
                val += _p[r - 1][n - 1];
                _p[r][n] = val;
            }
        }
    }
}

/* public interface required by the test harness */
seq_t a(idx_t n)
{
    if (n < 6)
        return (seq_t)0;

    _init();

    int64_t sum = 0;                     /* use signed so negative terms work */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t k1 = d;
            idx_t k2 = n / d;
            seq_t term1 = _p[6][n / k1];
            seq_t term2 = _p[6][n / k2];

            if (_mu[k1] == 1)
                sum += (int64_t)term1;
            else if (_mu[k1] == -1)
                sum -= (int64_t)term1;

            if (k2 != k1) {
                if (_mu[k2] == 1)
                    sum += (int64_t)term2;
                else if (_mu[k2] == -1)
                    sum -= (int64_t)term2;
            }
        }
    }
    return (seq_t)sum;
}
