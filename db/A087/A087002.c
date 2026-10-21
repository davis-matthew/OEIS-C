/*
 * A087002  Right half of periodic part of decimal expansion of 1/p for those
 *          primes having a periodic part of even length.
 * https://oeis.org/A087002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   get_prime, period_length, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A087002
   a(n) = right half of the periodic part of the decimal expansion of 1/p
   for the n‑th prime p whose period length is even.
   The period is defined as the minimal k>0 with 10^k ≡ 1 (mod p).
   To obtain the “right half” the underlying period string is rotated
   to start with a non‑zero digit, then split into two equal halves
   and the latter half is returned as an arbitrary precision integer.
   The function recomputes the value on demand, suitable for any
   call order and n ≥ 1.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"   /* get_prime(idx_t) */
#include "../utils/seq_from_str.h"  /* seq_from_str(const char *) */
#include "../utils/period_length.h"

static inline seq_t compute_a_for_prime(uint64_t p)
{
    uint32_t L = period_length(p);     /* period length */
    if ((L & 1u) != 0)                 /* not even – caller must skip */
        return 0;

    /* Generate the digits of the period (starting from remainder 1). */
    char *digits = (char *)malloc(L);
    if (!digits) return 0; /* allocation failed */

    uint64_t r = 1;
    for (uint32_t i = 0; i < L; ++i) {
        uint64_t d = (r * 10) / p;
        r = (r * 10) % p;
        digits[i] = '0' + (int)d;
    }

    /* Find first non‑zero digit to rotate the period. */
    uint32_t first_non_zero = 0;
    while (first_non_zero < L && digits[first_non_zero] == '0')
        ++first_non_zero;

    /* If all digits were zero (impossible for prime > 5) give 0. */
    if (first_non_zero == L) {
        free(digits);
        return 0;
    }

    /* Determine the right half of the rotated string. */
    uint32_t half = L / 2;
    uint32_t start = (first_non_zero + half) % L;
    char *right = (char *)malloc(half + 1);
    if (!right) {
        free(digits);
        return 0;
    }

    for (uint32_t i = 0; i < half; ++i)
        right[i] = digits[(start + i) % L];
    right[half] = '\0';

    seq_t result = seq_from_str(right);

    free(digits);
    free(right);
    return result;
}

seq_t a(idx_t n)
{
    /* Find the n-th prime with an even period length and return
       the right half of its decimal period. */
    idx_t count = 0;
    idx_t p_index = 1;  /* primes are 1‑based indexed by get_prime */

    while (1) {
        uint64_t prime = get_prime(p_index++);

        /* Skip p = 2, 5 which have non‑even periods. */
        if (prime == 2 || prime == 5)
            continue;

        uint32_t L = period_length(prime);
        if ((L & 1u) != 0)      /* only even periods */
            continue;

        ++count;
        if (count == n)
            return compute_a_for_prime(prime);
    }
}
