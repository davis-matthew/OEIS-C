/*
 * A047973  Distance of n-th prime to nearest cube.
 * https://oeis.org/A047973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = distance from the nth prime to the nearest integer cube
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 92 terms through n = 948 (2026-11-01)
 * Helpers:   ipow_i32, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/is_prime.h"

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range, any value is acceptable */

    /* Cache of primes.  We grow it lazily so each value is computed only once. */
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;   /* number of primes stored */
    static size_t prime_cap = 0;   /* capacity of the primes array */
    static uint64_t last_prime = 0;/* largest prime generated so far */

    /* Generate primes until we have at least n of them. */
    while (prime_cnt < n) {
        uint64_t candidate = last_prime + 1;
        /* Find the next prime (trivial trial division is sufficient for our limits). */
        while (!is_prime(candidate))
            ++candidate;

        /* Grow the array if necessary. */
        if (prime_cnt == prime_cap) {
            size_t new_cap = prime_cap ? prime_cap * 2 : 8;
            /* Ensure we don't overflow the capacity in size_t just in case. */
            ensure_capacity(new_cap, &prime_cap, (void **)&primes, sizeof(uint64_t));
        }

        primes[prime_cnt++] = candidate;
        last_prime = candidate;
    }

    /* The nth prime (indexing from 1). */
    uint64_t p = primes[n - 1];
    seq_t sp = (seq_t)p;

    /* Floor cube root of the prime. */
    seq_t c = floor_cube_root(sp);

    /* Compute the two neighbouring cubes. */
    seq_t cube_low  = c * c * c;
    seq_t c_next    = c + 1;
    seq_t cube_high = c_next * c_next * c_next;

    /* Distances to the two cubes. */
    seq_t diff_low  = sp >= cube_low ? sp - cube_low : cube_low - sp;
    seq_t diff_high = cube_high >= sp ? cube_high - sp : sp - cube_high;

    /* Return the smaller distance. */
    return diff_low < diff_high ? diff_low : diff_high;
}
