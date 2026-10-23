/*
 * A048569  Values of k for which the number of divisors of the central
 *          binomial coefficient C(k, floor(k/2)) exceeds the number of
 *          divisors of all other binomial coefficients C(k,j).
 * https://oeis.org/A048569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 45, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A048569: k such that the divisor count of the central 
   binomial coefficient C(k, floor(k/2)) exceeds that of all other
   binomial coefficients in the k‑th row of Pascal's triangle.
   This implementation generates the sequence on demand, caching
   computed terms.  The algorithm:
     - For each k, build the sequence of divisor counts of all
       binomial coefficients C(k,j) from j=0..k by incremental
       factorisation of the ratio C(k,j)=C(k,j-1)*(k-j+1)/j.
     - The exponent vector of primes is updated using the smallest
       prime factor array (spf).  The divisor count is maintained
       multiplicatively: when the exponent of a prime changes from
       e to e+Δ, the divisor count is multiplied by (e+Δ+1)/(e+1).
     - The central binomial coefficient indices are pos1 = k/2
       and pos2 = (k+1)/2 (equal when k is even).  The divisor
       count at pos1 is the central divisor count.  If any other
       divisor count is greater than or equal to this central value,
       the row does not satisfy the property.
     - All such k are inserted into a cache; the function a(n)
       returns the n‑th element of this cache (offset 1).
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/* global data structures                                       */
/* ------------------------------------------------------------ */

static size_t current_limit = 0;      /* sieve limit */
static int *spf = NULL;              /* smallest prime factor */
static int *primeIdx = NULL;         /* prime index mapping (size limit+1) */
static int *primes = NULL;           /* list of primes up to limit */
static size_t primes_count = 0;      /* number of primes */

/* cache of sequence values */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_capacity = 0;
static idx_t next_k = 1;             /* next k to test */

/* ------------------------------------------------------------ */
/* Helper: sieve & prime tables                                 */
/* ------------------------------------------------------------ */

static void ensure_limit(size_t limit)
{
    if (limit <= current_limit) return;

    if (spf) free(spf);
    if (primeIdx) free(primeIdx);
    if (primes) free(primes);

    spf = (int*)malloc((limit + 1) * sizeof(int));
    primeIdx = (int*)malloc((limit + 1) * sizeof(int));

    for (size_t i = 0; i <= limit; ++i) spf[i] = (int)i;

    for (size_t i = 2; i * i <= limit; ++i)
    {
        if (spf[i] == (int)i)
        {
            for (size_t j = i * i; j <= limit; j += i)
                if (spf[j] == (int)j)
                    spf[j] = (int)i;
        }
    }

    /* collect primes */
    primes_count = 0;
    primes = (int*)malloc((limit + 1) * sizeof(int));  /* upper bound */
    for (size_t i = 2; i <= limit; ++i)
    {
        if (spf[i] == (int)i)
        {
            primeIdx[i] = (int)primes_count;
            primes[primes_count++] = (int)i;
        }
        else
        {
            primeIdx[i] = -1;
        }
    }

    primes = (int*)realloc(primes, primes_count * sizeof(int));
    current_limit = limit;
}

/* ------------------------------------------------------------ */
/* Apply factorisation of an integer x to the exponent vector   */
/* ------------------------------------------------------------ */

static inline void apply_factor(int x, int delta,
                                int *exp, seq_t *divcnt,
                                const int *sp, const int *pIdx)
{
    if (x == 1) return;
    while (x > 1)
    {
        int p = sp[x];
        int e = 0;
        while (x % p == 0)
        {
            x /= p;
            ++e;
        }
        int idx = pIdx[p];
        int olde = exp[idx];
        int newe = olde + delta * e;
        /* update divisor count: remove old factor, add new */
        *divcnt /= (seq_t)(olde + 1);
        exp[idx] = newe;
        *divcnt *= (seq_t)(newe + 1);
    }
}

/* ------------------------------------------------------------ */
/* Determine if k satisfies the property                       */
/* ------------------------------------------------------------ */

static bool is_good_k(idx_t k)
{
    size_t K = (size_t)k;
    /* ensure we have prime tables */
    if (K > current_limit)
        ensure_limit(K);

    /* exponent vector for current binomial */
    int *exp = (int*)calloc(primes_count, sizeof(int));
    seq_t divcnt = 1;

    /* allocate array of divisor counts for the row */
    seq_t *divcounts = (seq_t*)malloc((K + 1) * sizeof(seq_t));
    divcounts[0] = 1;

    /* build row */
    for (size_t j = 1; j <= K; ++j)
    {
        apply_factor((int)(K - j + 1), +1, exp, &divcnt, spf, primeIdx);
        apply_factor((int)j, -1, exp, &divcnt, spf, primeIdx);
        divcounts[j] = divcnt;
    }

    /* central indices */
    size_t pos1 = K / 2;
    size_t pos2 = pos1;
    if (K % 2 == 1) pos2 = pos1 + 1;

    seq_t central = divcounts[pos1];

    bool good = true;
    for (size_t j = 0; j <= K; ++j)
    {
        if (j == pos1 || j == pos2) continue;
        if (divcounts[j] >= central)
        {
            good = false;
            break;
        }
    }

    free(exp);
    free(divcounts);
    return good;
}

/* ------------------------------------------------------------ */
/* Grow cache until it contains at least nth term               */
/* ------------------------------------------------------------ */

static void fill_cache(idx_t n)
{
    while (cache_size < n)
    {
        if (is_good_k(next_k))
        {
            if (cache_size >= cache_capacity)
            {
                cache_capacity = cache_capacity ? cache_capacity * 2 : 16;
                cache = (seq_t*)realloc(cache, cache_capacity * sizeof(seq_t));
            }
            cache[cache_size++] = (seq_t)next_k;
        }
        ++next_k;
    }
}

/* ------------------------------------------------------------ */
/* Public function required by the problem                       */
/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, but safe */
    fill_cache(n);
    return cache[n - 1];
}
