/*
 * A154988  Odd numbers that are the right-angle adjacent side in more than
 *          one primitive Pythagorean triple with prime hypotenuse.
 * https://oeis.org/A154988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 27, u32 n <= 32, u64 n <= 6000,
 *            u128 n <= 6000, big any n
 * Helpers:   gcd_idx_t, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A154988 by enumerating odd
   integers and testing for more than one primitive
   Pythagorean triple with prime hypotenuse. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd_idx_t.h"
#include "../utils/is_prime_big.h"

/* Check whether an odd integer a is the adjacent side of
   more than one primitive Pythagorean triple whose
   hypotenuse is prime. */
static bool has_multiple_prime_hypotenuse(uint64_t a)
{
    uint64_t count = 0;
    for (uint64_t d1 = 1; d1 <= a / d1; d1 += 2) {    /* odd divisors only */
        if (a % d1 != 0)
            continue;
        uint64_t d2 = a / d1;
        if (d1 >= d2)               /* we have considered all unordered pairs */
            break;
        if (gcd_idx_t(d1, d2) != 1) /* gcd(m,n)=1 ⇔ gcd(d1,d2)=1 */
            continue;
        uint64_t m = (d1 + d2) >> 1; /* (d1+d2)/2 */
        uint64_t n = (d2 - d1) >> 1; /* (d2-d1)/2 */
        seq_t hyp = (seq_t)m * m + (seq_t)n * n;
        if (is_prime_big(hyp)) {
            ++count;
            if (count >= 2)
                return true;
        }
    }
    return false;
}

/* Static cache for the sequence */
static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_a   = 1;   /* next odd integer to test */

/* OEIS entry */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; 0 is unused */
        return 0;

    /* Ensure the cache contains at least n terms */
    while (cache_len < (size_t)n) {
        /* Test the current odd integer */
        if (has_multiple_prime_hypotenuse(next_a)) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)next_a;
        }

        /* Advance to the next odd integer */
        if (next_a > UINT64_MAX - 2)        /* avoid overflow */
            break;
        next_a += 2;
    }

    /* Return the requested term */
    return cache[(size_t)n - 1];
}
