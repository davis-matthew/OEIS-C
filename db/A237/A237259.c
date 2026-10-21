/*
 * A237259  Least positive integer k < n such that prime(k) + 2 and
 *          prime(prime(n-k)) + 2 are both prime, or 0 if such a number k does
 *          not exist.
 * https://oeis.org/A237259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1576, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A237259: least k < n such that prime(k)+2 and prime(prime(n‑k))+2 are both prime,
   or 0 if no such k. Uses a sieve up to 2,100,000 to obtain the needed primes.
*/
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define MAX_SIEVE   2100000
#define MAX_PRIMES  250000

/* Sieve and prime list */
static uint8_t  isPrimeArr[MAX_SIEVE + 1];
static uint32_t primesArr[MAX_PRIMES + 1];
static size_t   primeCount = 0;
static bool     ready      = false;

static void
build_sieve(void)
{
    if (ready) return;

    /* initialise */
    for (size_t i = 0; i <= MAX_SIEVE; ++i) isPrimeArr[i] = 1;
    isPrimeArr[0] = isPrimeArr[1] = 0;

    /* simple Eratosthenes */
    for (size_t i = 2; i * i <= MAX_SIEVE; ++i) {
        if (isPrimeArr[i]) {
            for (size_t j = i * i; j <= MAX_SIEVE; j += i)
                isPrimeArr[j] = 0;
        }
    }

    /* build prime index array (1‑based) */
    primeCount = 0;
    for (size_t i = 2; i <= MAX_SIEVE; ++i) {
        if (isPrimeArr[i]) {
            if (primeCount < MAX_PRIMES) {
                ++primeCount;
                primesArr[primeCount] = (uint32_t)i;
            }
        }
    }
    ready = true;
}

seq_t
a(idx_t n)
{
    if (!ready) build_sieve();

    if (n <= 1) return 0;

    for (idx_t k = 1; k < n; ++k) {
        idx_t pk  = (idx_t)primesArr[(size_t)k];          /* prime(k)          */
        idx_t nmk = n - k;                                /* n – k             */
        if (nmk < 1 || nmk > (idx_t)(primeCount + 1)) continue;
        idx_t pnk = (idx_t)primesArr[(size_t)nmk];        /* prime(n‑k)        */

        /* test prime(pk + 2) */
        if (pk + 2 > MAX_SIEVE || !isPrimeArr[pk + 2]) continue;

        /* test prime(prime(pnk) + 2) */
        idx_t ppnk = (idx_t)primesArr[(size_t)pnk];       /* prime(prime(n‑k)) */
        if (ppnk + 2 > MAX_SIEVE || !isPrimeArr[ppnk + 2]) continue;

        return (seq_t)k;
    }
    return 0;
}
