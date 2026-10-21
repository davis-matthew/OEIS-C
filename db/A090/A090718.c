/*
 * A090718  Let p run through the primes; write p in base 10 and then
 *          interpret it in base 128 getting a number q; if q is prime then
 *          adjoin q to the sequence.
 * https://oeis.org/A090718
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the nth number (n ≥ 2) obtained by taking each prime p
 *            in increasing order, writing p in base 10 and interpreting the
 *            same decimal digits as a base 128 integer q; if q is prime, q is
 *            added to the sequence. The sequence starts at a(2)=2 (offset 2)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 21, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* For indices below the offset the return value is irrelevant. */
    if (n < 2) return (seq_t)0;

    /* Static cache of the sequence values. */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t size = 0;          /* number of terms currently stored */

    /* Static generator state for primes. */
    static uint64_t prime_candidate = 2;   /* current candidate for is_prime_small */

    /* Initialise cache on first call. */
    if (!cache) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        size = 0;
    }

    /* Generate terms until we have at least (n‑1) terms (indices 0…n‑2). */
    size_t needed = (size_t)(n - 1);
    while (size < needed) {
        /* Find next prime p. */
        uint64_t p;
        if (prime_candidate == 2) {
            p = 2;
            prime_candidate = 3;
        } else {
            while (!is_prime_small((idx_t)prime_candidate))
                prime_candidate += 2;        /* skip even numbers */
            p = prime_candidate;
            prime_candidate += 2;
        }

        /* Convert decimal digits of p into base‑128 integer q. */
        uint8_t digits[20];
        size_t len = 0;
        uint64_t tmp = p;
        while (tmp > 0) {
            digits[len++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }
        if (len == 0) digits[len++] = 0;   /* never happens for primes */

        seq_t q = 0;
        for (size_t i = len; i-- > 0; ) {
            q = q * (seq_t)128 + (seq_t)digits[i];
        }

        /* If q is prime, store it. */
        if (is_prime_big(q)) {
            if (size == cap) {
                cap <<= 1;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[size++] = q;
        }
    }

    /* a(2) is the first element of cache, so return cache[n‑2]. */
    return cache[n - 2];
}
