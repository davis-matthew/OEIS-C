/*
 * A107135  Primes of the form 5x^2 + 6y^2.
 * https://oeis.org/A107135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 822, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isprime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of primes that can be expressed as 5*x^2+6*y^2
   (equivalently, primes congruent to 5, 11, 29, 59 or 101 modulo 120).
   The following function finds the n‑th such prime by repeatedly
   testing candidates that satisfy the congruence condition. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/isprime.h"   /* provides bool isprime(uint64_t) */

seq_t a(idx_t n)
{
    static uint64_t *primes = NULL;   /* array of primes already found */
    static size_t   cap     = 0;      /* current capacity of the array   */
    static size_t   cnt     = 0;      /* number of primes stored         */
    static uint64_t last    = 4;      /* last number examined            */

    /* Grow the list until it contains n primes of the required form. */
    while (cnt < n) {
        /* Find next candidate that satisfies the modulus 120 condition. */
        for (;;) {
            ++last;
            uint64_t r = last % 120U;
            if (r != 5U && r != 11U && r != 29U && r != 59U && r != 101U)
                continue;
            if (isprime(last))
                break;   /* last is now the next prime of the desired form */
        }

        /* Ensure sufficient space in the array. */
        if (cnt == cap) {
            size_t newcap = cap == 0 ? 8 : cap * 2U;
            uint64_t *tmp = (uint64_t *)realloc(primes, newcap * sizeof(uint64_t));
            if (!tmp)
                abort();   /* allocation failure – stop execution */
            primes = tmp;
            cap = newcap;
        }

        primes[cnt++] = last;   /* store the found prime */
    }

    return (seq_t)primes[n - 1];
}
