/*
 * A142123  Primes congruent to 14 mod 37.
 * https://oeis.org/A142123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 188, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A142123 – primes congruent to 14 mod 37.
 * The value a(n) is the n‑th prime p with p % 37 == 14 (offset 1).
 * This implementation searches the arithmetic progression 37*k+14
 * and tests each candidate for primality.  The results are memoised
 * so that calls with any ordering of n remain O(n) after the
 * requested index is reached.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"

static inline bool test_prime(uint64_t cand)
{
    /* decide which primality routine to use based on the size of seq_t */
    if (sizeof(seq_t) <= 8)            /* seq_t is a built‑in unsigned integer */
        return is_prime(cand);
    else                              /* seq_t is an arbitrary‑precision integer */
        return is_prime_big((seq_t)cand);
}

seq_t a(idx_t n)
{
    if (n == 0)                 /* offset is 1, values for n < 1 are undefined */
        return 0;

    /* static state shared between calls */
    static seq_t *primes = NULL;   /* array of found primes */
    static size_t cap = 0;         /* capacity of this array */
    static size_t cnt = 0;         /* number of primes stored */
    static uint64_t lastC = 0;     /* last candidate tested (37*k+14) */

    /* first call: initialise storage and starting candidate */
    if (primes == NULL) {
        cap = 16;
        primes = (seq_t *)malloc(cap * sizeof(seq_t));
        lastC = 199ULL - 37ULL;    /* start before the first valid prime */
    }

    /* generate primes until we have at least n of them */
    while (cnt < n) {
        lastC += 37ULL;            /* next candidate in the progression */
        if (test_prime(lastC)) {
            if (cnt == cap) {      /* grow array on demand */
                cap *= 2;
                seq_t *tmp = (seq_t *)realloc(primes, cap * sizeof(seq_t));
                if (!tmp) {        /* allocation failure – abort */
                    free(primes);
                    primes = NULL;
                    cap = 0;
                    cnt = 0;
                    return 0;
                }
                primes = tmp;
            }
            primes[cnt++] = (seq_t)lastC;
        }
    }

    return primes[n - 1];
}
