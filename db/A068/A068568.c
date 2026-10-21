/*
 * A068568  Let S = a subset of {n,n+1,n+2,..,n+m}, number of parts of S is
 *          more than 1. a(n) = smallest number m such that the product of
 *          parts of S is a square.
 * https://oeis.org/A068568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest m such that a subset of {n,…,n+m}, with at least
 *            two elements, has product a perfect square
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a linear basis over GF(2) built from the
 * square‑free (parity) factorisations of the integers.  The first time
 * an integer in the range is found whose parity vector is linearly
 * dependent on the previously seen ones (or a second perfect square is
 * encountered), the corresponding offset m is returned.  The code
 * precomputes the smallest prime factor and prime indices up to a
 * fixed limit; that limit is chosen large enough for all calls used
 * in the tests (50000).  All arrays have static bound sizes, so the
 * implementation is fully compliant with C11 and C++17 without
 * relying on variable‑length arrays or new/delete. */

#include <stdint.h>
#include <stdbool.h>
#include <string.h> /* for memset */

/* ------------------------------------------------------------------ */
/*  Global tables                                             */
/* ------------------------------------------------------------------ */
#define LIMIT        50000            /* upper bound for numbers that may be factored */
#define MAX_PRIMES   6000            /* number of primes below LIMIT (≈5133)  */
#define WORDS_MAX    ((MAX_PRIMES+63)/64)

static int      spf[LIMIT+1];          /* smallest prime factor */
static int      primeIndex[LIMIT+1];   /* index of prime in primes[] */
static uint32_t primes[MAX_PRIMES];    /* list of primes */
static int      numPrimes;             /* count of primes found */
static int      wordsCount;            /* ceil(numPrimes/64) */

/* Linear basis storage.  Each basis vector is an array of WORDS_MAX 64‑bit
 * words that encodes the parity vector of a number.  basisPivot[i] holds
 * the index (0‑based) of the highest‑set bit of basis[i]. */
static uint64_t basis[MAX_PRIMES][WORDS_MAX];
static int      basisPivot[MAX_PRIMES];
static int      basisSize;             /* number of vectors currently in the basis */

/* ------------------------------------------------------------------ */
/*  Simple sieve for smallest prime factor and prime list     */
/* ------------------------------------------------------------------ */
static void init(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    memset(spf, 0, sizeof spf);
    memset(primeIndex, -1, sizeof primeIndex);
    numPrimes = 0;

    for (int i = 2; i <= LIMIT; ++i) {
        if (!spf[i]) {
            spf[i] = i;
            primeIndex[i] = numPrimes;
            primes[numPrimes++] = (uint32_t)i;
            if ((long long)i * i <= LIMIT) {
                for (int j = i * i; j <= LIMIT; j += i)
                    if (!spf[j]) spf[j] = i;
            }
        }
    }
    wordsCount = (numPrimes + 63) >> 6;   /* ceil division by 64 */
}

/* ------------------------------------------------------------------ */
/*  Helper: construct parity vector (bitset of odd exponents)   */
/* ------------------------------------------------------------------ */
static void get_parity_mask(uint32_t x, uint64_t *mask)
{
    /* zero initial mask */
    for (int w = 0; w < wordsCount; ++w)
        mask[w] = 0;

    while (x > 1) {
        uint32_t p = spf[x];
        int exp = 0;
        while (x % p == 0) {
            x /= p;
            exp ^= 1;          /* keep only odd exponent */
        }
        if (exp) {
            int idx = primeIndex[p];
            int w   = idx >> 6;
            int b   = idx & 63;
            mask[w] ^= (1ULL << b);
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Helper: get highest set bit index in a mask, or -1 if zero     */
/* ------------------------------------------------------------------ */
static int highest_bit(const uint64_t *v)
{
    for (int w = wordsCount - 1; w >= 0; --w) {
        uint64_t word = v[w];
        if (word) {
            return (w << 6) + (63 - __builtin_clzll(word));
        }
    }
    return -1;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init();                    /* ensure tables are ready */
    if (n == (idx_t)0) return (seq_t)1;   /* a(0)=1 */

    /* buffers for masks – fixed size arrays (WORDS_MAX) */
    static uint64_t currMask[WORDS_MAX];
    static uint64_t temp[WORDS_MAX];

    bool zeroSeen = false;      /* whether we have already seen a square */
    basisSize = 0;              /* clear basis for this call */

    /* iterate over k = n, n+1, … until a dependent subset is found */
    for (uint64_t k = (uint64_t)n; ; ++k) {
        /* skip numbers beyond precomputed limit */
        if (k > (uint64_t)LIMIT) {
            /* If this happens, we cannot factor reliably; dispatch an
             * error sentinel (here simply return 0).  The test suite
             * should never reach this point. */
            return (seq_t)0;
        }
        get_parity_mask((uint32_t)k, currMask);

        /* Check for perfect square (zero vector) */
        int hi = highest_bit(currMask);
        if (hi < 0) {
            if (zeroSeen) return (seq_t)(k - n);  /* second square found */
            zeroSeen = true;
            continue;          /* need at least two elements */
        }

        /* Copy current mask into temp for reduction */
        for (int w = 0; w < wordsCount; ++w)
            temp[w] = currMask[w];

        /* Reduce temp against current basis */
        while (true) {
            hi = highest_bit(temp);
            if (hi < 0) {          /* reduced to zero → dependent subset */
                return (seq_t)(k - n);
            }
            /* Search for a basis vector with the same pivot */
            bool found = false;
            for (int i = 0; i < basisSize; ++i) {
                if (basisPivot[i] == hi) {
                    for (int w = 0; w < wordsCount; ++w)
                        temp[w] ^= basis[i][w];
                    found = true;
                    break;
                }
            }
            if (!found) {           /* independent – add to basis */
                for (int w = 0; w < wordsCount; ++w)
                    basis[basisSize][w] = temp[w];
                basisPivot[basisSize++] = hi;
                break;               /* done with this k, move to next */
            }
        }
    }
}
