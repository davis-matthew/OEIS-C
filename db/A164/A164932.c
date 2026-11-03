/*
 * A164932  Prime numbers in which the sum of the largest digit and the
 *          smallest digit is also a prime.
 * https://oeis.org/A164932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 69 terms through n = 897 (2026-11-03)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A164932: primes p for which the sum of the largest
 * and smallest digit is also a prime.  The implementation
 * generates the required primes on demand and caches them.
 * The cache is kept in static storage and is grown as needed.
 * The function works for any build that defines seq_t and idx_t. */
#include <stdbool.h>
#include <stddef.h>   /* size_t, NULL */
#include <stdlib.h>   /* realloc, malloc */
#include <stdint.h>   /* uint64_t */

#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"

/* Helper: compute the sum of the smallest and largest digit of a prime. */
static int digit_sum_minmax(uint64_t p)
{
    int min_d = 9, max_d = 0;
    while (p > 0) {
        int d = (int)(p % 10);
        if (d < min_d) min_d = d;
        if (d > max_d) max_d = d;
        p /= 10;
    }
    return min_d + max_d;
}

/* Helper: check whether a small integer (≤18) is prime. */
static bool is_small_prime_int(int s)
{
    /* primes ≤ 18 */
    static const int primes[] = {2,3,5,7,11,13,17};
    for (size_t i = 0; i < sizeof(primes)/sizeof(primes[0]); ++i)
        if (s == primes[i]) return true;
    return false;
}

/* Core function: return the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t sz = 0;
    static uint64_t next = 2;   /* next integer to test for primality */

    /* Ensure cache can hold 'n' elements. */
    if (sz < (size_t)n) {
        size_t new_cap = cap ? cap : 4;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) {   /* allocation failed */
            return (seq_t)0;
        }
        cache = tmp;
        cap = new_cap;
    }

    /* Generate primes until we have enough cached. */
    while (sz < (size_t)n) {
        /* Skip even numbers after 2 for efficiency. */
        if (next != 2 && (next & 1) == 0) {
            ++next;
            continue;
        }

        if (is_prime_seq(next)) {
            int s = digit_sum_minmax(next);
            if (is_small_prime_int(s)) {
                cache[sz++] = (seq_t)next;
            }
        }
        ++next;
    }

    return cache[(size_t)n - 1];
}
