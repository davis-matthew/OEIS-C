/*
 * A140141  Positions of second appearances of primes in A039649.
 * https://oeis.org/A140141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 3512, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: precompute primes and Euler's totient values up to a fixed
   limit.  For every prime p (index n>1) the term a(n) is the smallest
   composite k with φ(k)=p−1.  The array of pre‑computed answers is
   filled by scanning all composites once.  a(1)=2. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* (Any helper prototypes are available in the build environment.) */

static const idx_t LIMIT_PRIME = 3000000U;    /* covers many thousands of indices */
static const idx_t LIMIT_PHI   = LIMIT_PRIME * 2ULL;   /* φ values up to 2·LIMIT_PRIME */

static bool          initialized   = false;
static idx_t        *primeIdx      = NULL;   /* prime value → 1‑based index (0 if composite) */
static uint8_t      *isComposite   = NULL;   /* composite flag for numbers up to LIMIT_PHI */
static uint32_t     *phiUint       = NULL;   /* Euler's totient for numbers up to LIMIT_PHI */
static idx_t        *primes        = NULL;   /* list of primes up to LIMIT_PHI */
static size_t       primeCnt       = 0;      /* number of primes up to LIMIT_PRIME */
static uint32_t     *ans           = NULL;   /* minimal composite for each prime index */

/* Linear sieve: fills isComposite, phiUint, primes and primeIdx. */
static void init(void)
{
    if (initialized) return;
    initialized = true;

    /* Allocate arrays.  Sizes are chosen to be inexpensive in memory. */
    isComposite = (uint8_t*)calloc((size_t)(LIMIT_PHI + 1), sizeof(uint8_t));
    phiUint     = (uint32_t*)calloc((size_t)(LIMIT_PHI + 1), sizeof(uint32_t));
    primeIdx    = (idx_t*)    calloc((size_t)(LIMIT_PRIME + 1), sizeof(idx_t));
    primes      = (idx_t*)    malloc((size_t)(LIMIT_PHI / 10) * sizeof(idx_t)); /* enough for >127k primes */

    if (!isComposite || !phiUint || !primeIdx || !primes) {
        abort();   /* allocation failed */
    }

    /* Linear sieve up to LIMIT_PHI */
    size_t cnt = 0;
    for (idx_t i = 2ULL; i <= LIMIT_PHI; ++i) {
        if (!isComposite[i]) {
            primes[cnt++] = i;
            if (i <= LIMIT_PRIME) {
                primeIdx[i] = (idx_t)(cnt);   /* 1‑based index */
            }
            phiUint[i] = (uint32_t)(i - 1ULL);
        }
        for (size_t j = 0; j < cnt; ++j) {
            idx_t p = primes[j];
            idx_t m = i * p;
            if (m > LIMIT_PHI) break;
            isComposite[m] = 1U;
            if (i % p == 0U) {
                phiUint[m] = phiUint[i] * (uint32_t)p;
                break;
            } else {
                phiUint[m] = phiUint[i] * (uint32_t)(p - 1ULL);
            }
        }
    }

    primeCnt = cnt;

    /* Allocate answer array (1‑based). */
    ans = (uint32_t*)calloc((size_t)(primeCnt + 1), sizeof(uint32_t));
    if (!ans) abort();

    /* For each composite k, if φ(k)+1 is a prime, store the first such k
       (the smallest composite achieving φ(k)+1). */
    for (idx_t k = 4ULL; k <= LIMIT_PHI; ++k) {
        if (isComposite[k]) {
            uint32_t ph = phiUint[k];
            if ((uint64_t)ph + 1ULL <= LIMIT_PRIME) {
                idx_t idx = primeIdx[ph + 1ULL];
                if (idx != 0ULL && ans[idx] == 0U) {
                    ans[idx] = (uint32_t)k;
                }
            }
        }
    }
}

/* Public function required by the judge. */
seq_t a(idx_t n)
{
    init();  /* ensure tables are ready */

    if (n == 0ULL) return (seq_t)0;   /* out of OEIS offset */
    if (n == 1ULL) return (seq_t)2;   /* a(1) = 2 is special */

    if (n > (idx_t)primeCnt) {
        /* Out of pre‑computed range – return 0 (no value requested). */
        return (seq_t)0;
    }

    uint32_t v = ans[(size_t)n];
    return (seq_t)v;
}
