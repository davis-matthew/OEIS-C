/*
 * A229106  Prime time display in hours, minutes, seconds on a six-digit
 *          24-hour digital clock.
 * https://oeis.org/A229106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 2531, u32 n <= 7669, u64 n <= 7669,
 *            u128 n <= 7669, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A229106: prime times on a 24‑hour digital clock.
   Each time hh:mm:ss is interpreted as the integer hh*10000+mm*100+ss
   and is counted if it is prime and the padded digits correspond to a
   valid time (00‑23 hours, 00‑59 minutes, 00‑59 seconds). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t *primeTimes = NULL;   /* array of primes that are valid times */
static size_t primeTimesCount = 0; /* number of entries in primeTimes */

/* Build the list of prime times the first time a(n) is called. */
static void init_prime_times(void)
{
    if (primeTimes != NULL)
        return; /* already built */

    /* 24 h × 60 min × 60 s = 86400 different times,
       but the largest hhmmss value is 23:59:59 → 235959. */
    const size_t LIMIT = 235959;          /* inclusive upper bound for primes */

    /* Allocate a sieve array of bool.  LIMIT + 1 elements. */
    bool *isPrime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!isPrime)
        return; /* allocation failed – leave primeTimes NULL */

    /* Initialise sieve: true for primes, false for composites. */
    for (size_t i = 0; i <= LIMIT; ++i)
        isPrime[i] = true;
    isPrime[0] = false;
    isPrime[1] = false;

    for (size_t p = 2; p * p <= LIMIT; ++p) {
        if (isPrime[p]) {
            for (size_t q = p * p; q <= LIMIT; q += p)
                isPrime[q] = false;
        }
    }

    /* First pass: count how many primes correspond to valid times. */
    size_t cnt = 0;
    for (size_t n = 2; n <= LIMIT; ++n) {
        if (!isPrime[n])
            continue;
        unsigned hour   = n / 10000;
        unsigned minute = (n / 100) % 100;
        unsigned second = n % 100;
        if (hour < 24 && minute < 60 && second < 60)
            ++cnt;
    }

    /* Allocate storage for the resulting list. */
    primeTimes = (seq_t *)malloc(cnt * sizeof(seq_t));
    if (!primeTimes) {
        free(isPrime);
        return; /* allocation failed */
    }

    /* Second pass: fill the array. */
    size_t pos = 0;
    for (size_t n = 2; n <= LIMIT; ++n) {
        if (!isPrime[n])
            continue;
        unsigned hour   = n / 10000;
        unsigned minute = (n / 100) % 100;
        unsigned second = n % 100;
        if (hour < 24 && minute < 60 && second < 60)
            primeTimes[pos++] = (seq_t)n;
    }

    primeTimesCount = cnt;
    free(isPrime);
}

/* Sequence function for A229106.  Index n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for offset 1; return 0 safely */
        return (seq_t)0;

    init_prime_times();
    if (primeTimes == NULL || (size_t)n > primeTimesCount)
        return (seq_t)0;  /* out of range */

    return primeTimes[(size_t)(n - 1)];
}
