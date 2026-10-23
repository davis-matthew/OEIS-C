/*
 * A134729  Concatenation of next n Sophie Germain primes A005384(n).
 * https://oeis.org/A134729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the concatenation of the next n Sophie Germain * primes.
 *            The SG primes are indexed starting at 1. The first prime * used
 *            for a(n) is the prime with index ((n-1)*n)/2 + 1. This *
 *            implementation memoises all found SG primes so that repeated
 *            calls * with arbitrary n are efficient
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   digit_len, is_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

/* Global storage for the Sophie Germain primes found so far. */
static uint64_t *sg_primes = NULL;
static size_t     sg_count  = 0;
static size_t     sg_cap    = 0;
static uint64_t   last_checked = 2;      /* next number to test */

/* Ensure room for one more entry and store p. */
static void add_sg_prime(uint64_t p)
{
    if (sg_count >= sg_cap) {
        size_t new_cap = sg_cap ? sg_cap * 2 : 64;
        void *tmp = realloc(sg_primes, new_cap * sizeof(uint64_t));
        if (!tmp) abort(); /* memory allocation failure */
        sg_primes = (uint64_t *)tmp;
        sg_cap    = new_cap;
    }
    sg_primes[sg_count++] = p;
}

/* --------------------------------------------------------------- */
/*  a(n)  -------------------------------------------------------- */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Determine which block of SG primes we need.  Indices are 1‑based. */
    idx_t start = ((n - 1) * n) / 2 + 1;
    idx_t end   = start + n - 1;

    /* Generate SG primes until we have at least 'end' of them. */
    while (sg_count < (size_t)end) {
        for (uint64_t p = last_checked; ; ++p) {
            /* Fast ≡2 mod 3 test (optional optimisation). */
            if (p % 3 == 0 && p != 3) continue;
            if (is_prime((idx_t)p) && is_prime((idx_t)(2 * p + 1))) {
                add_sg_prime(p);
                if (sg_count >= (size_t)end) {
                    last_checked = p + 1;
                    break; /* enough primes for this call */
                }
            }
        }
    }

    /* Concatenate the required block of SG primes. */
    seq_t result = 0;
    for (idx_t i = start - 1; i < end; ++i) {
        seq_t part = (seq_t)sg_primes[i];
        int   d    = digit_len(part);
        seq_t pow  = pow10((idx_t)d);
        result = result * pow + part;
    }
    return result;
}
