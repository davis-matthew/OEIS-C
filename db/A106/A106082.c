/*
 * A106082  Primes p such that 5*p+14 and 14*p+5 are primes.
 * https://oeis.org/A106082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that 5*p+14 and 14*p+5 are primes.
   Computed by searching primes on demand. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

#define MAX_TERMS 20000  /* sufficiently large for n ≈ 10000 */

seq_t a(idx_t n)
{
    /* static state persists between calls */
    static seq_t terms[MAX_TERMS];
    static size_t terms_cnt    = 0;
    static uint64_t next_prime = 3;  /* start scanning odd numbers ≥3 */
    static bool initialized    = false;
    if (!initialized) {
        initialized = true;
    }

    /* generate terms until we have at least n of them */
    while (terms_cnt < (size_t)n) {
        uint64_t p = next_prime;

        /* check if candidate is prime */
        if (is_prime(p)) {
            uint64_t ap = 5ULL * p + 14ULL;
            uint64_t bp = 14ULL * p + 5ULL;
            if (is_prime(ap) && is_prime(bp)) {
                if (terms_cnt < MAX_TERMS) {
                    terms[terms_cnt++] = (seq_t)p;
                } else {
                    /* capacity exceeded – cannot produce more terms */
                    return (seq_t)0;
                }
            }
        }

        /* advance to the next odd number; guard against overflow */
        if (p >= UINT64_MAX - 2) {
            break;
        }
        next_prime = p + 2;
    }

    if (n <= terms_cnt) {
        return terms[n - 1];
    }
    /* should not happen for valid n */
    return (seq_t)0;
}
