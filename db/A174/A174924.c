/*
 * A174924  Semiprimes sp(k) = q * r such that sum of digits of sp(k) equals
 *          sum of digits of the semiprime index k.
 * https://oeis.org/A174924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   a(n) = the n‑th semiprime whose digit sum equals * the digit sum
 *            of its index among all semiprimes. * The code builds a list of
 *            primes once, then scans natural * numbers, checks semiprimality
 *            by trial division with that * prime list, and caches the
 *            qualifying semiprimes for fast * repeated querie...
 * Range:     u8 n <= 8, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/digit_sum.h"

/* ---------- prime generation ------------------------------------------ */
#define PRIME_SIEVE_LIMIT 5000000ULL           /* enough for n ≈ 10000 */
#define MAX_PRIMES 400000                      /* bound on #primes below sieve limit */

static uint32_t primes[MAX_PRIMES];
static size_t   primeCnt = 0;
static bool     primes_built = false;

/* build the primes array once */
static void build_primes(void)
{
    if (primes_built) return;

    /* sieve of Eratosthenes, using a dynamic allocation for the composite flags */
    unsigned char *isComposite = (unsigned char *)calloc(PRIME_SIEVE_LIMIT + 1, 1);
    if (!isComposite) return;   /* out of memory – program will fail later gracefully */

    for (uint32_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i) {
        if (!isComposite[i]) {
            if (primeCnt < MAX_PRIMES)
                primes[primeCnt++] = i;
            if ((uint64_t)i * (uint64_t)i <= PRIME_SIEVE_LIMIT) {
                for (uint32_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                    isComposite[j] = 1;
            }
        }
    }
    free(isComposite);
    primes_built = true;
}

/* ---------- semiprime test -------------------------------------------- */
static bool is_semiprime(uint64_t x)
{
    if (x < 4) return false;                /* smallest semiprime = 2*2 */
    uint64_t tmp = x;
    int factorCount = 0;

    for (size_t pi = 0; pi < primeCnt && (uint64_t)primes[pi] * (uint64_t)primes[pi] <= tmp; ++pi) {
        uint32_t p = primes[pi];
        while (tmp % p == 0) {
            ++factorCount;
            tmp /= p;
            if (factorCount > 2) return false;
        }
    }
    if (tmp > 1) ++factorCount;      /* remaining prime factor */
    return factorCount == 2;
}

/* ---------- cache for computed a(n) ----------------------------------- */
static seq_t *cache = NULL;
static idx_t  cacheCap = 0;          /* allocated number of slots */
static idx_t  matchedCount = 0;      /* number of terms actually stored */

static void cache_ensure(idx_t needed)
{
    if (needed <= cacheCap) return;
    idx_t newCap = cacheCap ? cacheCap << 1 : 256;
    while (newCap < needed) newCap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
    if (!tmp) abort();    /* out of memory */
    cache = tmp;
    cacheCap = newCap;
}

/* ---------- persistent state ------------------------------------------ */
static uint64_t current_num = 2;      /* next integer to test globally */
static idx_t    semiprimeIndex = 0;   /* current semiprime index */

/* ---------- main sequence function ------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is 1‑indexed */
    build_primes();
    cache_ensure(n);

    /* generate terms until we have at least n matches */
    while (matchedCount < n) {
        uint64_t x = current_num++;
        if (!is_semiprime(x)) continue;   /* skip non‑semiprimes */

        ++semiprimeIndex;                 /* increment semiprime counter */

        if (digit_sum((seq_t)x) == digit_sum((seq_t)semiprimeIndex)) {
            cache[matchedCount++] = (seq_t)x;
        }
    }

    return cache[n - 1];
}
