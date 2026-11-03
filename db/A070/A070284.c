/*
 * A070284  Smallest of 4 consecutive numbers each divisible by a square.
 * https://oeis.org/A070284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m such that m, m+1, m+2, m+3 are each divisible
 *            by a perfect square > 1
 * Range:     u8 n <= 1, u16 n <= 151, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 51 terms through n = 9383 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation scans increasing integers, marking numbers that are not square‑free by a sieve of squares,
   then counts quadruples of consecutive non‑square‑free numbers until the nth occurrence is found. */
/* The approach runs in O(n) time with a small, growing buffer, comfortably fast for n up to at least 10 000. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Compute the nth term of A070284.  n >= 1. */
static seq_t find_nth_term(idx_t n)
{
    /* Start with a generous upper bound; the density of quadruples of non‑square‑free numbers is about 1/42,
       so 80×n gives a good safety margin. */
    size_t bound = (size_t)(n * 80) + 1000;
    if (bound < 2000) bound = 2000;                     /* safety floor */
    const size_t extra = 3;                            /* need to access bound+3 safely */

    for (;;) {
        /* Allocate array where bad[i]==1 means i is divisible by a perfect square >1. */
        uint8_t *bad = (uint8_t*)calloc(bound + extra + 1, 1);
        if (!bad) abort();   /* memory failure – the test harness will kill the program */

        /* Sieve primes up to sqrt(bound+3). */
        size_t lim = (size_t)sqrt((double)(bound + extra)) + 1;
        bool *isPrime = (bool*)calloc(lim + 1, 1);
        if (!isPrime) { free(bad); abort(); }

        /* Simple Eratosthenes sieve. */
        for (size_t i = 2; i <= lim; ++i) isPrime[i] = true;
        for (size_t i = 2; i * i <= lim; ++i) {
            if (isPrime[i]) {
                for (size_t j = i * i; j <= lim; j += i)
                    isPrime[j] = false;
            }
        }

        /* Mark all numbers that are divisible by a square. */
        for (size_t p = 2; (uint64_t)p * (uint64_t)p <= bound + extra; ++p) {
            if (!isPrime[p]) continue;
            uint64_t p2 = (uint64_t)p * (uint64_t)p;
            for (uint64_t j = p2; j <= bound + extra; j += p2)
                bad[j] = 1;
        }

        /* Scan for consecutive quadruples of bad numbers. */
        size_t count = 0;
        uint64_t result = 0;
        for (size_t i = 1; i + 3 <= bound; ++i) {
            if (bad[i] && bad[i + 1] && bad[i + 2] && bad[i + 3]) {
                ++count;
                if (count == n) {
                    result = (uint64_t)i;
                    break;
                }
            }
        }

        /* Clean up temporary arrays. */
        free(bad);
        free(isPrime);

        if (count >= n) return (seq_t)result;   /* found the term */
        bound <<= 1;                            /* expand search window and retry */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n=0 – return 0 */
    return find_nth_term(n);
}
