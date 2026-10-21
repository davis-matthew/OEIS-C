/*
 * A086924  Primes such that sum of the first and last digits is a square.
 * https://oeis.org/A086924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   is_square_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This code computes OEIS A086924: primes p for which the sum of the first
   and last digits of p is a perfect square.  It uses a fixed sieve of
   Eratosthenes up to 10 000 000 to pre‑compute the first 10 000 terms
   (more are added by trial division if required).  The first value is
   a(1)=2, as required by the offset.  All arithmetic that fits in
   native integers is kept within 64 bit bounds; values are stored as
   seq_t which is compatible with both fixed‑width and arbitrary
   precision integer types. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/is_square_sum.h"

#define SIEVE_LIMIT 10000000UL

static bool initialized = false;
static seq_t *qual_primes = NULL;
static size_t qual_count = 0;

/* compute the nth term (n>=1) of A086924 */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;               /* safety for forbidden index */

    if (!initialized) {
        /* ---- create a sieve of Eratosthenes up to SIEVE_LIMIT ---- */
        uint8_t *isprime = (uint8_t*)malloc(SIEVE_LIMIT + 1);
        if (!isprime) return (seq_t)0;         /* unlikely, but keep safe */
        for (size_t i = 0; i <= SIEVE_LIMIT; ++i) isprime[i] = 1;
        isprime[0] = isprime[1] = 0;
        for (uint32_t i = 2; (uint64_t)i * i <= SIEVE_LIMIT; ++i) {
            if (isprime[i]) {
                for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += i)
                    isprime[j] = 0;
            }
        }
        /* ---- collect qualifying primes ---- */
        size_t capacity = SIEVE_LIMIT / 100;           /* rough estimate */
        qual_primes = (seq_t*)malloc(capacity * sizeof(seq_t));
        if (!qual_primes) { free(isprime); return (seq_t)0; }
        for (uint32_t p = 2; p <= SIEVE_LIMIT; ++p) {
            if (!isprime[p]) continue;
            uint32_t last = p % 10;
            uint32_t first = p;
            while (first >= 10) first /= 10;
            if (is_square_sum((int)first + (int)last)) {
                if (qual_count >= capacity) {          /* grow array */
                    capacity <<= 1;
                    seq_t *tmp = (seq_t*)realloc(qual_primes,
                                                 capacity * sizeof(seq_t));
                    if (!tmp) { free(isprime); return (seq_t)0; }
                    qual_primes = tmp;
                }
                qual_primes[qual_count++] = (seq_t)p;
            }
        }
        free(isprime);
        initialized = true;
    }

    /* ---- answer the request ---- */
    if ((size_t)n <= qual_count) {
        return qual_primes[n - 1];
    }

    /* ---- iterate beyond pre‑computed range using trial division ---- */
    uint64_t candidate = SIEVE_LIMIT + 1;
    while (qual_count < (size_t)n) {
        /* primality test */
        bool prime = true;
        for (uint64_t d = 2; d * d <= candidate; ++d) {
            if (candidate % d == 0) { prime = false; break; }
        }
        if (prime) {
            uint64_t last = candidate % 10;
            uint64_t first = candidate;
            while (first >= 10) first /= 10;
            if (is_square_sum((int)first + (int)last)) {
                if (qual_count >= (size_t)qual_primes) { /* unlikely */
                    qual_count <<= 1;
                    seq_t *tmp = (seq_t*)realloc(qual_primes,
                                                 qual_count * sizeof(seq_t));
                    if (!tmp) return (seq_t)0;
                    qual_primes = tmp;
                }
                qual_primes[qual_count++] = (seq_t)candidate;
            }
        }
        ++candidate;
    }
    return qual_primes[n - 1];
}
