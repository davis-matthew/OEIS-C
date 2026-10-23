/*
 * A108226  Primes p such that p + 8^k is composite for all k >= 0.
 * https://oeis.org/A108226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p such that * p % 7 == 6, (i.e. p ≡ 6 (mod
 *            7)) * or p is one of the special primes 47 or 79
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A108226: primes p that for every k ≥ 0,   p + 8^k is composite. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static const uint64_t INITIAL_LIMIT = 2000000ULL;      /* initial sieve limit: 2 000 000 */
static const size_t    PRIME_CAP_INIT = 200000;        /* initial capacity for primes array */

static uint8_t  *isComposite = NULL;      // byte array for sieve flags
static uint64_t *primes      = NULL;      // array of primes up to current limit
static size_t    primeCnt    = 0;         // number of primes stored
static size_t    primeCap    = 0;         // capacity of primes array
static seq_t    *seqVals     = NULL;      // sequence values (primes satisfying the condition)
static size_t    seqCnt      = 0;         // number of sequence values stored
static size_t    seqCap      = 0;         // capacity of seqVals array
static uint64_t  nextCandidate = 0;       // next integer to test for primality
static bool      initialized = false;

/* Compute primes up to a given limit using the Eratosthenes sieve. */
static void generate_initial_primes(void)
{
    /* Allocate sieve array */
    isComposite = (uint8_t*)calloc(INITIAL_LIMIT + 1, sizeof(uint8_t));
    if (!isComposite) exit(1);

    /* Mark composites */
    isComposite[0] = isComposite[1] = 1;
    for (uint64_t i = 2; i * i <= INITIAL_LIMIT; ++i) {
        if (!isComposite[i]) {
            for (uint64_t j = i * i; j <= INITIAL_LIMIT; j += i)
                isComposite[j] = 1;
        }
    }

    /* Allocate primes array with some initial capacity */
    primeCap = PRIME_CAP_INIT;
    primes = (uint64_t*)malloc(primeCap * sizeof(uint64_t));
    if (!primes) exit(1);

    /* Fill primes array */
    for (uint64_t i = 2; i <= INITIAL_LIMIT; ++i) {
        if (!isComposite[i]) {
            if (primeCnt >= primeCap) {      /* grow if necessary */
                primeCap <<= 1;
                primes = (uint64_t*)realloc(primes, primeCap * sizeof(uint64_t));
                if (!primes) exit(1);
            }
            primes[primeCnt++] = i;
        }
    }

    /* Build the sequence values from the primes already found */
    seqCap = primeCnt / 3 + 10;          /* rough estimate of required capacity */
    seqVals = (seq_t*)malloc(seqCap * sizeof(seq_t));
    if (!seqVals) exit(1);

    for (size_t i = 0; i < primeCnt; ++i) {
        uint64_t p = primes[i];
        if (p == 47 || p == 79 || (p % 7ULL) == 6ULL) {
            if (seqCnt >= seqCap) {        /* grow if necessary */
                seqCap <<= 1;
                seqVals = (seq_t*)realloc(seqVals, seqCap * sizeof(seq_t));
                if (!seqVals) exit(1);
            }
            seqVals[seqCnt++] = (seq_t)p;
        }
    }

    nextCandidate = primeCnt ? primes[primeCnt - 1] + 1ULL : 2ULL;
}

/* Generate additional primes beyond the initial limit if more sequence values
 * are required than were precomputed. */
static void extend_primes_and_sequence(void)
{
    while (seqCnt == 0 || (size_t)(seqVals[seqCnt - 1]) < seqCnt) {
        /* test nextCandidate for primality using stored primes */
        bool isPrime = true;
        for (size_t i = 0; i < primeCnt; ++i) {
            uint64_t p = primes[i];
            unsigned __int128 sq = (unsigned __int128)p * p;
            if (sq > nextCandidate) {
                break;
            }
            if (nextCandidate % p == 0) {
                isPrime = false;
                break;
            }
        }

        if (isPrime) {
            /* store the prime */
            if (primeCnt >= primeCap) {
                primeCap <<= 1;
                primes = (uint64_t*)realloc(primes, primeCap * sizeof(uint64_t));
                if (!primes) exit(1);
            }
            primes[primeCnt++] = nextCandidate;

            /* test if it satisfies the sequence condition */
            if (nextCandidate == 47 || nextCandidate == 79 ||
                ((nextCandidate % 7ULL) == 6ULL)) {
                if (seqCnt >= seqCap) {
                    seqCap <<= 1;
                    seqVals = (seq_t*)realloc(seqVals, seqCap * sizeof(seq_t));
                    if (!seqVals) exit(1);
                }
                seqVals[seqCnt++] = (seq_t)nextCandidate;
            }
        }
        ++nextCandidate;
    }
}

/* Ensure at least n terms of the sequence are precomputed. */
static void ensure_n_terms(idx_t n)
{
    if (!initialized) {
        generate_initial_primes();
        initialized = true;
    }
    while (seqCnt < (size_t)n) {
        extend_primes_and_sequence();
    }
}

/* The required external function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n=0, as per OEIS offset */
    ensure_n_terms(n);
    return seqVals[n - 1];
}
