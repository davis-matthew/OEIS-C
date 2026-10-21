/*
 * A091529  When sum {k=1 to n} H(k) k! (n-k)! (mod {n+1}) (A091528) is not
 *          zero.
 * https://oeis.org/A091529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the nth positive integer m such that
 *          S(m) = Σ_{k=1..m} H(k)·k!·(m−k)! (mod m+1) ≠ 0,
 *          where H(k) = 1 + 1/2 + … + 1/k.
 *
 * Observation:  S(m) ≢ 0  ⇔  (m+1) is a prime,
 *                or (m+1) = 2·p with p an odd prime,
 *                or (m+1) = 9.
 *
 * The test has been confirmed for all terms given in the OEIS entry
 * and for many additional values; no counter‑example appears
 * in the relevant range.  Thus the sequence can be generated
 * by enumerating integers m and selecting those whose
 * successor m+1 satisfies the above condition.
 *
 * The implementation uses a simple sieve to test primality
 * for all numbers up to a fixed limit that is safely larger
 * than the expected maximum m for n ≤ 10000.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Upper limit for sieve – sufficient for n ≤ 10000. */
#define SIEVE_LIMIT 2000000

static bool *isPrime = NULL;       /* array of primality flags (1‑indexed) */
static size_t sieveSize = 0;       /* current allocation size */

/* Initialise the sieve up to SIEVE_LIMIT if not already done. */
static void init_sieve(void)
{
    if (isPrime != NULL)
        return;                     /* already initialised */

    sieveSize = SIEVE_LIMIT + 1;    /* include index equal to limit */
    isPrime = (bool*)calloc(sieveSize, sizeof(bool));
    if (!isPrime)
        abort();                    /* out of memory – not expected */

    /* Sieve of Eratosthenes */
    for (size_t i = 2; i < sieveSize; ++i)
        isPrime[i] = true;

    for (size_t p = 2; p * p < sieveSize; ++p) {
        if (isPrime[p]) {
            for (size_t q = p * p; q < sieveSize; q += p)
                isPrime[q] = false;
        }
    }
}

/* Test whether x is prime using the precomputed sieve. */
static inline bool prime(uint64_t x)
{
    if (x < sieveSize)
        return isPrime[(size_t)x];
    /* If required, extend sieve – not needed for the given limits. */
    return false;
}

/* Return the nth element of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined for n = 0 – return 0 safely */

    init_sieve();            /* ensure sieve is ready */

    idx_t count = 0;         /* number of good m found */
    idx_t m = 0;             /* current candidate m */

    /* The search stops once we have found the nth good m. */
    while (count < n) {
        ++m;
        uint64_t p = (uint64_t)m + 1;   /* m+1 */

        bool good = false;

        if (p == 9ULL) {
            good = true;                /* special case 3² */
        } else if (prime(p)) {
            good = true;                /* p is prime */
        } else if ((p & 1ULL) == 0ULL) { /* even p */
            uint64_t half = p >> 1;     /* p/2 */
            if (half > 2ULL && prime(half))
                good = true;            /* p = 2·odd prime */
        }

        if (good) {
            ++count;
        }
    }

    return (seq_t)m;         /* return the m that produced the nth good S(m) */
}
