/*
 * A100759  Beginning with 2, least prime not occurring earlier such that the
 *          concatenation of first n terms has the least prime factor
 *          prime(n).
 * https://oeis.org/A100759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, less, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A100759: a(n) = least unused prime p such that the least
   prime factor of the decimal concatenation of a(1)…a(n) equals prime(n). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static void init(void);
static int digits(uint32_t x);

/* pre–computed primes up to LIMIT */
#define LIMIT 200000               /* 2·10^5 > 10000th prime */
#define MAX_DIG 10                 /* maximum digits needed for any prime in this range */

static bool initialized = false;
static uint32_t *primes = NULL;    /* list of primes up to LIMIT */
static size_t primeCount = 0;      /* number of primes */
static bool *used = NULL;          /* whether a prime is already used in the sequence */
static uint32_t *Cmod = NULL;      /* current concatenation modulo every prime */
static uint32_t *pow10mod = NULL;  /* pow10mod[qi*(MAX_DIG+1)+d] = 10^d mod primes[qi] */

/* memoisation of computed terms */
static seq_t *memo = NULL;
static idx_t memoLen = 0;

static void init(void)
{
    if (initialized) return;
    initialized = true;

    /* Sieve of Eratosthenes */
    bool *isComp = (bool*)calloc(LIMIT + 1, sizeof(bool));
    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (!isComp[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i)
                isComp[j] = true;
        }
    }
    /* Count primes */
    for (size_t i = 2; i <= LIMIT; ++i)
        if (!isComp[i]) ++primeCount;

    primes = (uint32_t*)malloc(primeCount * sizeof(uint32_t));
    used = (bool*)calloc(primeCount, sizeof(bool));
    Cmod = (uint32_t*)calloc(primeCount, sizeof(uint32_t));

    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (!isComp[i]) {
            primes[idx++] = (uint32_t)i;
        }
    }
    free(isComp);

    /* Pre‑compute 10^d mod primes[qi] for d = 0..MAX_DIG */
    pow10mod = (uint32_t*)malloc(primeCount * (MAX_DIG + 1) * sizeof(uint32_t));
    for (size_t qi = 0; qi < primeCount; ++qi) {
        for (int d = 0; d <= MAX_DIG; ++d) {
            uint32_t pow = 1;
            for (int k = 0; k < d; ++k) {
                uint64_t t = (uint64_t)pow * 10u;
                pow = (uint32_t)(t % primes[qi]);
            }
            pow10mod[qi * (MAX_DIG + 1) + d] = pow;
        }
    }
}

/* number of decimal digits of a 32‑bit integer */
static int digits(uint32_t x)
{
    if (x >= 1000000000u) return 10;
    if (x >= 100000000u)  return 9;
    if (x >= 10000000u)   return 8;
    if (x >= 1000000u)    return 7;
    if (x >= 100000u)     return 6;
    if (x >= 10000u)      return 5;
    if (x >= 1000u)       return 4;
    if (x >= 100u)        return 3;
    if (x >= 10u)         return 2;
    return 1;
}

/* main sequence function */
seq_t a(idx_t n)
{
    init();

    if (n == 0) return 0;                     /* undefined, but harmless */

    /* extend memoisation array if necessary */
    if (n > memoLen) {
        size_t newSize = (size_t)n;
        memo = (seq_t*)realloc(memo, newSize * sizeof(seq_t));

        /* temporary array for computing new moduli */
        static uint32_t *tmpMod = NULL;
        if (!tmpMod) {
            tmpMod = (uint32_t*)malloc(primeCount * sizeof(uint32_t));
        }

        for (idx_t k = memoLen + 1; k <= n; ++k) {
            size_t targetIdx = (size_t)(k - 1);           /* index of prime(k) in primes[] */
            uint32_t targetPrime = primes[targetIdx];
            bool chosen = false;

            /* iterate over unused primes to find the smallest suitable one */
            for (size_t j = 0; j < primeCount && !chosen; ++j) {
                if (used[j]) continue;
                uint32_t candidate = primes[j];
                int d = digits(candidate);

                bool ok = true;
                /* compute all moduli and test LPD condition */
                for (size_t qi = 0; qi < primeCount; ++qi) {
                    uint32_t q = primes[qi];
                    uint32_t pow10 = pow10mod[qi * (MAX_DIG + 1) + d];
                    uint32_t new_mod = (uint32_t)(((uint64_t)Cmod[qi] * pow10 + (candidate % q)) % q);
                    tmpMod[qi] = new_mod;
                    if (q <= targetPrime) {
                        if (q < targetPrime && new_mod == 0) {
                            ok = false;        /* some smaller prime divides it */
                            break;
                        }
                        if (q == targetPrime && new_mod != 0) {
                            ok = false;        /* target prime does not divide it */
                            break;
                        }
                    }
                }
                if (!ok) continue;

                /* candidate satisfies the condition */
                chosen = true;
                used[j] = true;
                /* update current concatenation modulo each prime */
                for (size_t qi = 0; qi < primeCount; ++qi)
                    Cmod[qi] = tmpMod[qi];
                memo[k - 1] = (seq_t)candidate;
            }
            /* the sequence definition guarantees a solution exists */
        }
        memoLen = (size_t)n;
    }

    return memo[n - 1];
}
