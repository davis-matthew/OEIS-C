/*
 * A095680  13, together with primes of the form 10^k + 13 for some k >= 0.
 * https://oeis.org/A095680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   is_prime_big, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A095680: 13 followed by primes of the form 10^k + 13 (k ≥ 2). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/is_prime_u64.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static storage for the terms of the sequence. */
    static seq_t *terms   = NULL;
    static size_t size    = 0;
    static size_t cap     = 0;

    /* Static power of ten: 10^2 at start (k = 2). */
    static seq_t pow10     = (seq_t)100;   /* 10^2 */

    /* Initialise on first call. */
    if (size == 0) {
        cap    = 16;
        terms  = (seq_t*)malloc(cap * sizeof(seq_t));
        terms[0] = (seq_t)13;   /* a(1) = 13 */
        size = 1;
    }

    /* Generate terms until we have at least n of them. */
    while (size < (size_t)n) {
        /* Candidate = 10^k + 13.  pow10 already holds 10^k. */
        seq_t cand = pow10 + (seq_t)13;

        bool prime = false;

        /* Determine if cand fits in 64 bits for a faster test. */
        /* For unsigned types overflow wraps, so we check bounds
         * by comparing against the maximum value minus 13. */
        if (pow10 <= ((seq_t)((uint64_t)UINT64_MAX) - (seq_t)13)) {
            prime = is_prime_u64((uint64_t)cand);
        } else {
            prime = is_prime_big(cand);
        }

        if (prime) {
            if (size == cap) {
                cap *= 2;
                terms = (seq_t*)realloc(terms, cap * sizeof(seq_t));
            }
            terms[size++] = cand;
        }

        /* Advance to 10^(k+1).  Detect overflow for unsigned types. */
        seq_t prev = pow10;
        pow10 = pow10 * (seq_t)10;
        /* If multiplication wrapped, we cannot generate larger terms. */
        if (pow10 < prev) {
            /* No further powers of ten can be represented; stop. */
            break;
        }
    }

    return terms[(size_t)n - 1];
}
