/*
 * A186641  Primes that are not cyclic numbers (A001913).
 * https://oeis.org/A186641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   build_primes_upto, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime that is NOT a full‑reptend (cyclic) prime
   (i.e. the primes in A186641).  The sequence contains 2 and 5, and
   for every other prime p it is included iff 10 is NOT a primitive
   root modulo p.  The values are returned in 1‑based indexing. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/mod_pow.h"

/* --------------------------------------------------------------------- */
#define SMALL_PRIME_LIMIT 2000      /* used for factorisation of φ(p)   */
#define PRIME_LIMIT       2000000   /* enough for the first ~100 000 terms */

/* --------------------------------------------------------------------- */
static uint64_t smallPrimes[SMALL_PRIME_LIMIT];
static size_t   smallPrimeCnt = 0;

/* --------------------------------------------------------------------- */
static void init_small_primes(void)
{
    if (smallPrimeCnt > 0) return;          /* already initialised */

    bool *isComposite = (bool*)calloc(SMALL_PRIME_LIMIT + 1, sizeof(bool));
    if (!isComposite) return;               /* allocation failed */

    for (size_t i = 2; i <= SMALL_PRIME_LIMIT; ++i) {
        if (!isComposite[i]) {
            smallPrimes[smallPrimeCnt++] = i;
            if (i * i <= SMALL_PRIME_LIMIT) {
                for (size_t j = i * i; j <= SMALL_PRIME_LIMIT; j += i)
                    isComposite[j] = true;
            }
        }
    }
    free(isComposite);
}

/* --------------------------------------------------------------------- */
/* Return true iff 10 is a primitive root modulo p (p>5, p prime). */
/* For p=2 or 5 we treat as NOT a full‑reptend prime (they are in the
   sequence) so this routine is only called for p>5. */
static bool isFullReptendPrime(uint64_t p)
{
    if (p <= 5) return false;               /* will never be called? */

    uint64_t phi = p - 1;
    uint64_t remaining = phi;
    uint64_t primeFactors[64];
    size_t   pfCnt = 0;

    /* Factorise φ(p) using the pre‑computed small primes */
    for (size_t i = 0; i < smallPrimeCnt &&
                      (uint64_t)smallPrimes[i] * smallPrimes[i] <= remaining; ++i) {
        uint64_t q = smallPrimes[i];
        if (remaining % q == 0) {
            primeFactors[pfCnt++] = q;
            while (remaining % q == 0) remaining /= q;
        }
    }
    if (remaining > 1) primeFactors[pfCnt++] = remaining;

    /* Test whether the order of 10 modulo p equals φ(p) */
    for (size_t i = 0; i < pfCnt; ++i) {
        uint64_t q = primeFactors[i];
        seq_t pow = mod_pow((seq_t)10, (uint64_t)(phi / q), (seq_t)p);
        if (pow == (seq_t)1) return false;    /* order divides φ(p)/q */
    }
    return true;                                 /* 10 is a primitive root */
}

/* --------------------------------------------------------------------- */
static seq_t *primeSeq   = NULL;   /* array holding all desired primes    */
static size_t primeSeqLen = 0;     /* number of elements stored           */

/* --------------------------------------------------------------------- */
static void init_sequence(void)
{
    init_small_primes();

    uint64_t *primeList = NULL;
    size_t    primeCnt  = 0;
    build_primes_upto(PRIME_LIMIT, &primeList, &primeCnt);

#if defined(__cplusplus)
    primeSeq = new seq_t[primeCnt];
#else
    primeSeq = (seq_t*)malloc(primeCnt * sizeof(seq_t));
#endif
    size_t idx = 0;

    for (size_t i = 0; i < primeCnt; ++i) {
        uint64_t p = primeList[i];
        if (p == 2 || p == 5) {
            primeSeq[idx++] = (seq_t)p;
            continue;
        }
        if (!isFullReptendPrime(p))
            primeSeq[idx++] = (seq_t)p;
    }
    primeSeqLen = idx;

    free(primeList);
}

/* --------------------------------------------------------------------- */
/* Public accessor required by the test harness.  Returns the nth value
   of the sequence (1‑based).  If n is out of range, 0 is returned. */
seq_t a(idx_t n)
{
    static bool initialised = false;
    if (!initialised) {
        init_sequence();
        initialised = true;
    }

    if (n == 0 || (idx_t)primeSeqLen < n)
        return (seq_t)0;            /* undefined, safety fallback */

    return primeSeq[n - 1];
}
