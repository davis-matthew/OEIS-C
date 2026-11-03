/*
 * A098510  Row sums of a matrix associated to the inverse of a Catalan scaled
 *          binomial matrix.
 * https://oeis.org/A098510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=0..n} denominator((n+1)*C(n,k)/C(2n,n))
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 24 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below computes the denominator of the rational number
   (n+1)*C(n,k)/C(2n,n) by subtracting p-adic exponents, and then
   sums these denominators over k.  The implementation uses only
   integer arithmetic and a simple prime sieve to factor the
   small integers involved.  */

/* Standard headers – no C++11/14/17 specific features are used. */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Update the exponent array exps by adding 'sign' times the exponent
   of all prime factors of val.  sign is +1 for addition, -1 for
   subtraction.  primeIndex maps prime value -> its index in primes. */
static void
factor_update(uint64_t val, int sign, const int *primes, int P,
              int *exps, const int *primeIndex)
{
    uint64_t m = val;
    for (int i = 0; i < P && (uint64_t)primes[i] * (uint64_t)primes[i] <= m; ++i) {
        int p = primes[i];
        int e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }
        if (e) {
            exps[i] += sign * e;
        }
    }
    if (m > 1) {                  /* m is now prime and <= limit */
        int idx = primeIndex[(int)m];
        if (idx >= 0) {
            exps[idx] += sign;    /* exponent 1 */
        }
    }
}

/* Legendre's formula: exponent of prime p in N!. */
static int
legendre_idx_t(idx_t N, int p)
{
    int e = 0;
    while (N) {
        N /= p;
        e += (int)N;
    }
    return e;
}

/* Helper: generate all primes <= limit and return them in primesOut
   and the number of primes in *cntOut.  The caller must free the
   returned array. */
static void
sieve_primes_idx_t(idx_t limit, int **primesOut, int *cntOut)
{
    if (limit < 2) {
        *primesOut = NULL;
        *cntOut = 0;
        return;
    }

    size_t sz = (size_t)limit + 1;
    unsigned char *isComposite = (unsigned char *)calloc(sz, sizeof(unsigned char));
    if (!isComposite) {
        *primesOut = NULL;
        *cntOut = 0;
        return;
    }

    /* Sieve. */
    for (size_t i = 2; i * i <= limit; ++i) {
        if (!isComposite[i]) {
            for (size_t j = i * i; j <= limit; j += i) {
                isComposite[j] = 1;
            }
        }
    }

    /* Count primes first. */
    int primesCnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!isComposite[i]) {
            ++primesCnt;
        }
    }

    /* Allocate array and fill it. */
    int *primes = (int *)malloc((size_t)primesCnt * sizeof(int));
    if (!primes) {
        free(isComposite);
        *primesOut = NULL;
        *cntOut = 0;
        return;
    }

    int idx = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!isComposite[i]) {
            primes[idx++] = (int)i;
        }
    }

    free(isComposite);
    *primesOut = primes;
    *cntOut = primesCnt;
}

/* Forward declarations of helper functions. */
static void sieve_primes_idx_t(idx_t limit, int **primes, int *cnt);
static int legendre_idx_t(idx_t N, int p);
static void factor_update(uint64_t val, int sign, const int *primes, int P,
                          int *exps, const int *primeIndex);

/* Main function required by the problem statement. */
seq_t
a(idx_t n)
{
    /* n = 0 gives a(0) = 1. */
    if (n == 0)
        return (seq_t)1;

    /* Limit for prime sieve: need primes up to 2n. */
    idx_t limitIdx = n * 2;
    int *primes = NULL;
    int primeCnt = 0;
    sieve_primes_idx_t(limitIdx, &primes, &primeCnt);

    if (primeCnt == 0) {          /* only possible when n=0, handled above */
        free(primes);
        return (seq_t)1;
    }

    /* primeIndex[x] = index of prime value x in primes array, or -1. */
    int *primeIndex = (int *)malloc((size_t)(limitIdx + 1) * sizeof(int));
    if (!primeIndex) {
        free(primes);
        return (seq_t)0;          /* memory error – return 0 as safe fallback */
    }
    for (size_t i = 0; i <= (size_t)limitIdx; ++i)
        primeIndex[i] = -1;
    for (int i = 0; i < primeCnt; ++i)
        primeIndex[primes[i]] = i;

    /* Allocate exponent arrays. */
    int *expC2n = (int *)malloc((size_t)primeCnt * sizeof(int));
    int *expCk  = (int *)calloc((size_t)primeCnt, sizeof(int)); /* start with C(n,0)=1 */
    int *expN1  = (int *)malloc((size_t)primeCnt * sizeof(int));

    /* Exponents of (n+1). */
    for (int i = 0; i < primeCnt; ++i)
        expN1[i] = 0;
    factor_update((uint64_t)(n + 1), +1, primes, primeCnt, expN1, primeIndex);

    /* Exponents of C(2n,n) = (2n)!/(n!)^2. */
    for (int i = 0; i < primeCnt; ++i) {
        int p = primes[i];
        int e = legendre_idx_t(2 * n, p) - 2 * legendre_idx_t(n, p);
        expC2n[i] = e;
    }

    /* Sum of denominators. */
    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k) {
        /* Compute denominator for current k. */
        seq_t denom = 1;
        for (int i = 0; i < primeCnt; ++i) {
            int diff = expC2n[i] - (expCk[i] + expN1[i]);
            if (diff > 0) {
                seq_t pval = (seq_t)primes[i];
                for (int j = 0; j < diff; ++j) {
                    denom *= pval;
                }
            }
        }
        sum += denom;

        if (k == n)
            break;   /* finished all terms */

        /* Update exponents for C(n, k+1) from C(n, k). */
        uint64_t num = n - k;       /* factor (n-k)  */
        uint64_t den = k + 1;       /* factor (k+1)  */
        factor_update(num, +1, primes, primeCnt, expCk, primeIndex);
        factor_update(den, -1, primes, primeCnt, expCk, primeIndex);
    }

    /* Clean up allocated memory. */
    free(expC2n);
    free(expCk);
    free(expN1);
    free(primeIndex);
    free(primes);

    return sum;
}
