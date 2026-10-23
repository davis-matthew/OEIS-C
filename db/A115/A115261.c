/*
 * A115261  Prime numbers such that the absolute difference of the sum of
 *          their digits in odd positions and the sum of their digits in even
 *          positions is also a prime.
 * https://oeis.org/A115261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the n-th prime such that the absolute difference of the
 *            sum of its digits in odd positions and the sum in even positions
 *            is also a prime
 * Range:     u8 n <= 31, u16 n <= 2900, u32 n <= 1690, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a simple incremental search for primes
   and memorises the sequence in a static array.  The test for primality
   of a number is delegated to the helper from the utils directory:
   is_prime_big.  Digits are processed from right to left; the
   resulting absolute difference is evaluated as a prime using the same
   helper.  The function works for any supported seq_t and is
   independent of the order of calls. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* The OEIS offset is 1; for n == 0 we return 0 (undefined) */
    if (n == 0) return 0;

    /* static memoisation state */
    static seq_t *cache = NULL;      /* dynamic array of qualifying primes */
    static size_t cap = 0;           /* allocated capacity */
    static size_t used = 0;          /* number of elements stored */
    static seq_t last_prime = 1;     /* start just before the first prime (2) */

    /* grow storage if necessary */
    if (cap < (size_t)n) {
        size_t newcap = cap ? cap * 2 : 16;
        if (newcap < (size_t)n) newcap = n;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof *cache);
        if (!tmp) { /* allocation failure – abort */
            exit(EXIT_FAILURE);
        }
        cache = tmp;
        cap = newcap;
    }

    /* search for new qualifying primes until we have n terms */
    while (used < (size_t)n) {
        /* advance to next prime */
        do { ++last_prime; } while (!is_prime_big(last_prime));

        /* compute sum of digits in odd and even positions (from right) */
        seq_t tmp = last_prime;
        int odd_sum  = 0;
        int even_sum = 0;
        int pos = 1;               /* 1‑based from the least‑significant digit */
        while (tmp != 0) {
            int digit = (int)(tmp % 10);
            if (pos & 1) odd_sum += digit; else even_sum += digit;
            ++pos;
            tmp /= 10;
        }

        /* absolute difference */
        int diff = odd_sum - even_sum;
        if (diff < 0) diff = -diff;

        /* check if the difference is a prime (>1) */
        if (diff > 1 && is_prime_big((seq_t)diff)) {
            cache[used++] = last_prime;
        }
    }

    return cache[n - 1];
}
