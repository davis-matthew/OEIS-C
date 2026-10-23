/*
 * A191233  The number of times that the n-th digit of A154703(n) occurs in
 *          A154703(n).
 * https://oeis.org/A191233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A191233: for the binary concatenation of the first n primes,
   returns the number of occurrences of the n‑th digit in that concatenation. */

#include <stdint.h>
#include <stdlib.h>

/* Helper: binary length of a non‑zero uint64_t. */
static inline int bitlen_uint64(uint64_t x)
{
    int len = 0;
    while (x) {
        ++len;
        x >>= 1;
    }
    return len;
}

/* Test primality of a candidate with known primes (trial division). */
static int is_prime_candidate(uint64_t cand, const uint64_t *primes, size_t pcnt)
{
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > cand) break;
        if (cand % p == 0) return 0;
    }
    return 1;
}

/* Static state for primes and prefix sums. */
static uint64_t *primes      = NULL;   /* array of primes (uint64_t) */
static seq_t   *prefixLen   = NULL;   /* cumulative binary length */
static seq_t   *prefixOnes  = NULL;   /* cumulative number of 1s */
static size_t  primesCap    = 0;
static size_t  primesCnt    = 0;

/* Ensure the arrays can hold at least 'need' elements. */
static void expand_capacity(size_t need)
{
    if (primesCap >= need) return;
    size_t newCap = primesCap ? primesCap * 2 : 8;
    if (newCap < need) newCap = need;

    primes     = (uint64_t *)realloc(primes,     newCap * sizeof(uint64_t));
    prefixLen  = (seq_t   *)realloc(prefixLen,  newCap * sizeof(seq_t));
    prefixOnes = (seq_t   *)realloc(prefixOnes, newCap * sizeof(seq_t));
    primesCap  = newCap;
}

/* Generate primes until we have at least 'n' primes and update prefix sums. */
static void ensure_primes(idx_t n)
{
    /* We generate primes only up to n; n is the number of primes needed. */
    if (primesCnt >= (size_t)n) return;

    uint64_t cand = (primesCnt == 0) ? 2 : primes[primesCnt - 1] + 1;
    while (primesCnt < (size_t)n) {
        while (!is_prime_candidate(cand, primes, primesCnt)) ++cand;

        expand_capacity(primesCnt + 1);
        primes[primesCnt] = cand;

        int bits = bitlen_uint64(cand);
        int ones = __builtin_popcountll(cand);
        seq_t lenTmp  = (seq_t)bits;
        seq_t onesTmp = (seq_t)ones;

        if (primesCnt == 0) {
            prefixLen[primesCnt]  = lenTmp;
            prefixOnes[primesCnt] = onesTmp;
        } else {
            prefixLen[primesCnt]  = prefixLen[primesCnt - 1] + lenTmp;
            prefixOnes[primesCnt] = prefixOnes[primesCnt - 1] + onesTmp;
        }

        ++primesCnt;
        cand += 1;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* defined only for n >= 1 */

    /* Ensure we have primes and prefix sums up to n. */
    ensure_primes(n);

    /* Total counts for all n primes. */
    seq_t totalOnes  = prefixOnes[n - 1];
    seq_t totalLen   = prefixLen[n - 1];

    /* Find the prime that contains the n‑th digit (1‑based position). */
    size_t k = 0;
    while (prefixLen[k] < (seq_t)n) ++k;   /* k is 0‑based index */

    /* Offset of the digit within this prime. */
    size_t prevLen = (k == 0) ? 0 : (size_t)prefixLen[k - 1];
    size_t offset  = (size_t)n - prevLen;   /* 1‑based within the prime */

    /* Determine the bit at that position. */
    uint64_t prime   = primes[k];
    int bitsLen      = bitlen_uint64(prime);
    size_t shift     = bitsLen - offset;
    int digit        = (prime >> shift) & 1U;

    return (digit == 1) ? totalOnes : (totalLen - totalOnes);
}
