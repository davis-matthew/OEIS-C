/*
 * A195416  Primes p such that there is only one semiprime number between
 *          prime(p) and prime(p+1).
 * https://oeis.org/A195416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A195416: a(n) is the n-th prime index p such that the interval
 * (prime(p), prime(p+1)) contains exactly one semiprime.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* forward declaration of helper arrays */
static uint32_t *spf = NULL;          /* smallest prime factor */
static uint32_t *primes = NULL;       /* list of primes */
static idx_t   *qualifiers = NULL;    /* list of qualifying prime indices */

/* storage for sizes */
static size_t spf_size = 0;
static size_t primes_cnt = 0;
static size_t qualifiers_cnt = 0;
static size_t qualifiers_cap = 0;
static bool initialized = false;

/* test if a positive integer is a semiprime using the spf array */
static inline bool is_semiprime(uint32_t n)
{
    uint32_t cnt = 0;
    while (n > 1) {
        uint32_t f = spf[n];
        ++cnt;
        n /= f;
        if (cnt > 2) return false;
    }
    return cnt == 2;
}

/* build sieve, primes list and qualifying indices */
static void build_sieve(void)
{
    const size_t LIMIT = 30000000;            /* 30 million, enough for 10 000 terms */
    spf_size = LIMIT + 1;
    spf = (uint32_t *)malloc(spf_size * sizeof(uint32_t));
    if (!spf) exit(1);
    for (size_t i = 0; i < spf_size; ++i) spf[i] = 0;

    /* classic Eratosthenes sieve with SPF */
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i > LIMIT / i) continue;
            for (size_t j = i * i; j <= LIMIT; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }

    /* collect primes */
    primes_cnt = 0;
    size_t primes_cap = 2000000;              /* initial capacity */
    primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    if (!primes) exit(1);
    for (size_t i = 2; i <= LIMIT; ++i)
        if (spf[i] == (uint32_t)i) {
            if (primes_cnt == primes_cap) {
                primes_cap <<= 1;
                primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!primes) exit(1);
            }
            primes[primes_cnt++] = (uint32_t)i;
        }

    /* find qualifying indices */
    qualifiers_cap = 1024;
    qualifiers = (idx_t *)malloc(qualifiers_cap * sizeof(idx_t));
    if (!qualifiers) exit(1);
    qualifiers_cnt = 0;

    for (size_t idx = 2; idx + 1 < primes_cnt; ++idx) {   /* idx is 1‑based prime index */
        if (spf[idx] == (uint32_t)idx) {                  /* idx itself is prime */
            uint32_t pval    = primes[idx - 1];
            uint32_t nextp   = primes[idx];
            int semcnt = 0;
            for (uint32_t n = pval + 1; n < nextp; ++n) {
                if (is_semiprime(n)) {
                    ++semcnt;
                    if (semcnt > 1) break;
                }
            }
            if (semcnt == 1) {
                if (qualifiers_cnt == qualifiers_cap) {
                    qualifiers_cap <<= 1;
                    qualifiers = (idx_t *)realloc(qualifiers, qualifiers_cap * sizeof(idx_t));
                    if (!qualifiers) exit(1);
                }
                qualifiers[qualifiers_cnt++] = (idx_t)idx;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (!initialized) {
        build_sieve();
        initialized = true;
    }
    if (n == 0) return 0;                    /* sequence is 1‑based */
    if ((size_t)n > qualifiers_cnt) return 0; /* out of bounds (should not happen) */
    return (seq_t)qualifiers[n - 1];
}
