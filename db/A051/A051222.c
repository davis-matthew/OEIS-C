/*
 * A051222  Numbers k such that Bernoulli number B_{k} has denominator 6.
 * https://oeis.org/A051222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A051222: numbers k such that Bernoulli number B_k has
   denominator 6.  An integer n is in the sequence iff
        1. n is even (so 2 and 3 are primes of the form d+1 where d∣n),
        2. for every divisor d of n (d≥2) the number d+1 is either
           not prime or equals 2 or 3.
   The function below builds the sequence on demand and caches the
   terms.  It works for any integer type seq_t (fixed‑width or
   arbitrary‑precision).  The routine is O(n·sqrt(n)) in the worst
   case, well within the required limits for n≤10 000. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

/* ------- small primality test ------------------------------------ */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1U) == 0 || n % 3U == 0) return false;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

/* ------- check whether an integer belongs to the sequence ------- */
static bool is_sequence_member(uint64_t n)
{
    if (n < 2) return false;
    if ((n & 1U) != 0)              /* must be even to contain 3 */
        return false;

    uint64_t r = (uint64_t)sqrt((double)n);
    for (uint64_t d = 1; d <= r; ++d) {
        if (n % d == 0) {
            /* divisor d */
            if (d >= 2) {
                uint64_t val = d + 1;
                if (val >= 5 && is_prime_uint64(val))
                    return false;
            }
            /* complementary divisor n/d */
            uint64_t d2 = n / d;
            if (d2 != d && d2 >= 2) {
                uint64_t val = d2 + 1;
                if (val >= 5 && is_prime_uint64(val))
                    return false;
            }
        }
    }
    return true;
}

/* ------- cache of computed terms -------------------------------- */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

static void ensure_cache_cap(size_t new_cap)
{
    if (cache_cap >= new_cap) return;
    size_t cap = cache_cap ? cache_cap : 8;
    while (cap < new_cap) cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    if (!tmp) exit(1);
    cache = tmp;
    cache_cap = cap;
}

/* ------- main sequence function --------------------------------- */
seq_t a(idx_t n)
{
    static uint64_t candidate = 1;          /* start searching after 1 */
    while (cache_size < (size_t)n) {
        ++candidate;
        if (is_sequence_member(candidate)) {
            ensure_cache_cap(cache_size + 1);
            cache[cache_size++] = (seq_t)candidate;
        }
    }
    return cache[n - 1];
}
