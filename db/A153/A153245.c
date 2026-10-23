/*
 * A153245  Numbers n>1 such that 6*n-7 is not prime.
 * https://oeis.org/A153245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth integer > 1 for which 6*n-7 is not prime
 * Range:     u8 n <= 130, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* simple primality test for 64‑bit values – fast enough for small 6*n‑7 */
static bool is_prime_u64(uint64_t v)
{
    if (v < 2)           return false;
    if (v % 2 == 0)      return v == 2;
    if (v % 3 == 0)      return v == 3;
    for (uint64_t i = 5; i * i <= v; i += 6) {
        if (v % i == 0)      return false;
        if (v % (i + 2) == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* dynamic cache of computed terms */
    static seq_t *cache = NULL;
    static size_t size = 0;   /* entries stored in cache   */
    static size_t cap  = 0;   /* allocated capacity         */
    static idx_t next  = 2;   /* next integer to test (≥2) */

    if (cache == NULL) {
        cap   = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    while (size < n) {
        /* find next integer with 6*i-7 composite */
        while (1) {
            uint64_t val = (uint64_t)6 * (uint64_t)next - 7;
            if (!is_prime_u64(val))
                break;
            ++next;
        }

        /* store found integer in cache */
        if (size >= cap) {
            cap = cap * 2;
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        }
        cache[size++] = (seq_t)next;
        ++next;
    }

    return cache[n - 1];
}
