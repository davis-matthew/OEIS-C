/*
 * A053180  Numbers ending in 7 which are not prime.
 * https://oeis.org/A053180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 4906, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers ending in 7 which are composite (non‑primes) */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* Simple deterministic prime test for 64‑bit integers */
static bool isPrime64(uint64_t n) {
    if (n < 2)               return false;
    if (n % 2 == 0)          return n == 2;
    if (n % 3 == 0)          return n == 3;
    uint64_t i = 5;
    uint64_t step = 2;
    while (i * i <= n) {
        if (n % i == 0)      return false;
        i += step;
        step = 6 - step;     /* alternate 2,4,... to test 5,7,11,13... */
    }
    return true;
}

/* Compute the OEIS sequence A053180 */
seq_t a(idx_t n)
{
    if (n == 0) /* undefined behaviour below offset */ return 0;

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t next_candidate = 27;   /* first composite ending in 7 */

    if (n > cache_len) {
        size_t required = n;
        if (required > cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 32 : cache_cap * 2;
            while (new_cap < required) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;                 /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        while (cache_len < required) {
            if (!isPrime64(next_candidate)) {
                cache[cache_len++] = (seq_t)next_candidate;
            }
            next_candidate += 10;              /* next number ending in 7 */
        }
    }

    return cache[n - 1];
}
