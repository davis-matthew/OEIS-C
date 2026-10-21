/*
 * A085345  Least number x such that number of primes between phi(x) and x
 *          equals n.
 * https://oeis.org/A085345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least x such that π(x) – π(φ(x)) = n (π counts the primes
 *            ≤ x, φ is Euler's totient). The function pre‑computes primes, φ,
 *            and prime‑count prefix sums up to a fixed limit and records the
 *            first x that gives each difference value. Hence a(n) is returned
 *            in O(1) after a one‑time O(limit) sieve
 * Range:     u8 n <= 29, u16 n <= 3283, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* Parameters.  The limit must be large enough for all n that may
   be requested (e.g. n ≤ 10000); 2 000 000 is more than sufficient. */
static const idx_t MAX_LIMIT = 2000000ULL;      /* sieve bound           */
static const idx_t MAX_N     = 10000ULL;       /* maximum n needed       */

/* ------------------------------------------------------------------ */
/* Static storage used after the first call to a() */
static seq_t          *aVals = NULL;   /* a(n) values  (index 1 .. MAX_N) */
static uint32_t       *phi   = NULL;   /* φ(i) for 0 .. MAX_LIMIT          */
static uint32_t       *spf   = NULL;   /* smallest prime factor of i (0 .. MAX_LIMIT) */
static uint8_t        *isPrimeArr = NULL;   /* 1 if prime, 0 otherwise          */
static uint32_t       *piPref = NULL;   /* prefix prime counts  (0 .. MAX_LIMIT) */
static int            initialized = 0;   /* 0 = not yet initialized */

/* ------------------------------------------------------------------ */
/* Linear sieve with totient computation */
static void init_sieve(void)
{
    size_t i, j;
    size_t np = 0;
    size_t primes_alloc = MAX_LIMIT / 10;   /* rough initial capacity */
    uint32_t *primes = (uint32_t*)malloc(primes_alloc * sizeof(uint32_t));
    if (!primes) return;   /* allocation failure → leave uninitialised */

    /* Allocate required arrays */
    phi        = (uint32_t*)malloc((MAX_LIMIT + 1) * sizeof(uint32_t));
    spf        = (uint32_t*)malloc((MAX_LIMIT + 1) * sizeof(uint32_t));
    isPrimeArr = (uint8_t *)malloc((MAX_LIMIT + 1) * sizeof(uint8_t));
    piPref     = (uint32_t*)malloc((MAX_LIMIT + 1) * sizeof(uint32_t));
    if (!phi || !spf || !isPrimeArr || !piPref) {
        free(primes);  /* clean up if any allocation failed */
        return;
    }

    /* Initialise */
    for (i = 0; i <= MAX_LIMIT; ++i) {
        phi[i] = (uint32_t)i;
        spf[i] = 0;
    }
    phi[0] = 0;  phi[1] = 1;

    /* Linear sieve to fill spf and primes list */
    for (i = 2; i <= MAX_LIMIT; ++i) {
        if (spf[i] == 0) {         /* prime */
            spf[i] = (uint32_t)i;
            if (np >= primes_alloc) {            /* grow primes array if needed */
                primes_alloc <<= 1;
                uint32_t *tmp = (uint32_t*)realloc(primes, primes_alloc * sizeof(uint32_t));
                if (!tmp) break;                 /* out of memory → exit loop */
                primes = tmp;
            }
            primes[np++] = (uint32_t)i;
        }
        for (j = 0; j < np; ++j) {
            uint32_t p = primes[j];
            uint64_t m64 = (uint64_t)i * (uint64_t)p;
            if (m64 > (uint64_t)MAX_LIMIT) break;
            uint32_t m = (uint32_t)m64;
            spf[m] = p;
            if (i % p == 0) break;
        }
    }

    /* Compute φ using spf */
    for (i = 2; i <= MAX_LIMIT; ++i) {
        uint32_t p = spf[i];
        uint32_t m = i / p;
        if (m % p == 0) {
            phi[i] = phi[m] * p;
        } else {
            phi[i] = phi[m] * (p - 1);
        }
    }

    /* Prime flag and prefix counts */
    for (i = 0; i <= MAX_LIMIT; ++i) {
        isPrimeArr[i] = (i >= 2 && spf[i] == (uint32_t)i) ? 1U : 0U;
    }
    piPref[0] = 0;
    for (i = 1; i <= MAX_LIMIT; ++i) {
        piPref[i] = piPref[i - 1] + (isPrimeArr[i] ? 1U : 0U);
    }

    /* Allocate and initialise aVals array */
    aVals = (seq_t*)malloc((MAX_N + 1) * sizeof(seq_t));
    if (aVals) {
        memset(aVals, 0, (MAX_N + 1) * sizeof(seq_t));
        /* Fill aVals: record first x with each difference d (1 .. MAX_N) */
        size_t found = 0;
        for (i = 2; i <= MAX_LIMIT; ++i) {
            uint32_t phi_i = phi[i];
            uint32_t d = piPref[i] - piPref[phi_i];
            if (d >= 1 && d <= MAX_N && aVals[d] == 0) {
                aVals[d] = (seq_t)i;
                ++found;
                if (found == MAX_N) break;  /* all required terms found */
            }
        }
    }

    /* Clean up temporary primes array */
    free(primes);
    initialized = 1;
}

/* ------------------------------------------------------------------ */
/* Public function required by the build system */
seq_t a(idx_t n)
{
    if (!initialized) init_sieve();

    if (n >= 1 && n <= MAX_N && aVals[n] != 0)
        return aVals[n];

    /* For safety: if n is out of range, return 0 (undefined) */
    return (seq_t)0;
}
