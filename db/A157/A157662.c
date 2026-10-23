/*
 * A157662  Composite numbers such that the last digit is equal to the sum of
 *          all the previous digits.
 * https://oeis.org/A157662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A157662
   a(n) = the nth composite number whose last digit equals the sum
   of all preceding digits, excluding repdigits such as 11, 22, 33 ...
   (e.g. 112, 123, 134, …) */
 
#include <stdlib.h>
#include <stdbool.h>

/* Prime test for seq_t values.
   Works for all integer sizes supported by the build (uint8_t … uint128_t
   and arbitrary‑precision types). */
static bool is_prime(seq_t n)
{
    if (n < 2)          return false;
    if (n % 2 == 0)     return n == 2;
    for (seq_t d = 3; d <= n / d; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

/* Test whether x satisfies the defining property of the sequence. */
static bool good(seq_t x)
{
    if (x < 10) return false;            /* at least two digits */

    /* last digit */
    seq_t last = x % 10;

    /* sum of all previous digits */
    seq_t sum = 0;
    seq_t t = x / 10;
    while (t > 0) {
        sum += t % 10;
        t /= 10;
    }
    if (sum != last) return false;

    /* exclude repdigits (all digits equal) */
    seq_t tmp = x;
    while (tmp >= 10) tmp /= 10;         /* most significant digit */
    int msd = (int)tmp;
    bool all_same = true;
    for (seq_t v = x; v > 0; v /= 10) {
        if ((int)(v % 10) != msd) {
            all_same = false;
            break;
        }
    }
    if (all_same) return false;

    /* must be composite */
    if (is_prime(x)) return false;

    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;

    /* candidate for testing */
    static seq_t candidate = 1;

    /* generate terms until we have at least n of them */
    while (cache_size < n) {
        candidate = candidate + 1;          /* next integer */

        if (!good(candidate))
            continue;

        /* store the found term */
        if (cache_size == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { exit(EXIT_FAILURE); }
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_size++] = candidate;
    }

    return cache[n - 1];
}
