/*
 * A050246  Digital clock primes.
 * https://oeis.org/A050246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 211, u32 n <= 211, u64 n <= 211,
 *            u128 n <= 211, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A050246 Digital clock primes.
   Formula: a(n) = A118848(n) + floor(A118848(n)/60) * 40
   (i.e. the n‑th prime among numbers of the form hour*100 + minute,
   with 0 ≤ hour ≤ 23 and 0 ≤ minute ≤ 59). */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Prefilter all candidates once and keep them in static storage. */
static seq_t *prime_vals = NULL;   /* array of primes */
static size_t prime_count = 0;     /* number of primes stored */
static bool initialized = false;

/* Test primality of a value ≤ 2359 (fits in uint64). */
static inline bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t i = 3; i * i <= x; i += 2) {
        if (x % i == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        const size_t MAX_CANDIDATES = 24 * 60;          /* 1440 possible times */
        prime_vals = (seq_t *)malloc(MAX_CANDIDATES * sizeof(seq_t));
        if (!prime_vals) return 0;                     /* allocation failed */

        size_t len = 0;
        for (uint16_t hour = 0; hour < 24; ++hour) {
            for (uint16_t minute = 0; minute < 60; ++minute) {
                uint16_t val = hour * 100 + minute;   /* 0 .. 2359 */
                if (is_prime_uint64(val)) {
                    prime_vals[len++] = (seq_t)val;   /* cast safe */
                }
            }
        }
        prime_count = len;
        initialized = true;
    }

    if (n == 0) return 0;                /* undefined for n = 0 */
    if ((idx_t)prime_count < n) return 0;/* out of range */
    return prime_vals[n - 1];
}
