/*
 * A079056  Numbers of prime triples (p,q,r), p<=q<=r, such that (p+q+r)
 *          divides n.
 * https://oeis.org/A079056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A068307(d) (A068307 = number of prime triples
 *            with sum d)
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *a_cache = NULL;              /* cached values a(1..n)             */
static idx_t cache_limit = 0;              /* last n for which cache is valid   */

/* ----  Helper: compute a(n) up to new_limit ---- */
static void compute_up_to(idx_t new_limit)
{
    /* Free previous cache if any */
    if (a_cache) {
        free(a_cache);
        a_cache = NULL;
    }

    size_t len = (size_t)(new_limit + 1);   /* length of arrays */

    /* ---- 1. Sieve primes up to new_limit ---- */
    int *isPrime = (int*)malloc((size_t)(new_limit + 1) * sizeof(int));
    for (size_t i = 0; i <= new_limit; ++i) isPrime[i] = 1;
    if (new_limit >= 0) isPrime[0] = 0;
    if (new_limit >= 1) isPrime[1] = 0;

    for (idx_t i = 2; i * i <= new_limit; ++i) {
        if (isPrime[i]) {
            for (idx_t j = i * i; j <= new_limit; j += i)
                isPrime[j] = 0;
        }
    }

    /* Count primes */
    int primeCount = 0;
    for (idx_t i = 2; i <= new_limit; ++i)
        if (isPrime[i]) ++primeCount;

    int *primes = (int*)malloc((size_t)primeCount * sizeof(int));
    for (idx_t i = 2, k = 0; i <= new_limit; ++i)
        if (isPrime[i]) primes[k++] = (int)i;

    free(isPrime);   /* done with isPrime */

    /* ---- 2. Count prime triples for each sum ---- */
    seq_t *triple = (seq_t*)calloc(len, sizeof(seq_t));   /* triple[s] = A068307(s) */
    for (int ii = 0; ii < primeCount; ++ii) {
        int p = primes[ii];
        for (int jj = ii; jj < primeCount; ++jj) {
            int q = primes[jj];
            int sum2 = p + q;
            if (sum2 > new_limit - 2) break;          /* minimal r is 2 */
            for (int kk = jj; kk < primeCount; ++kk) {
                int r = primes[kk];
                int sum = sum2 + r;
                if (sum > new_limit) break;
                triple[sum] += (seq_t)1;
            }
        }
    }

    /* ---- 3. Build divisor sum array a(n) ---- */
    seq_t *new_cache = (seq_t*)calloc(len, sizeof(seq_t));
    for (int d = 1; d <= new_limit; ++d) {
        seq_t val = triple[d];
        if (!val) continue;
        for (int m = d; m <= new_limit; m += d) {
            new_cache[m] += val;
        }
    }

    free(triple);
    free(primes);

    a_cache = new_cache;
    cache_limit = new_limit;
}

/* ----  Public function for the sequence ---- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                 /* offsets start at 1  */
    if (cache_limit < n) compute_up_to(n);       /* extend cache if needed */
    return a_cache[(size_t)n];
}
