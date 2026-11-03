/*
 * A140409  Prime factors of Lucas numbers.
 * https://oeis.org/A140409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th distinct prime that divides some Lucas number,
 *            ordered increasingly
 * Range:     u8 n <= 36, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* helper: extend the capacity of a dynamic array of seq_t */
static void extend_seq_array(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t newcap = (*cap) ? (*cap) * 2 : 16;
        while (newcap < needed) newcap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, newcap);
        *arr = tmp;
        *cap = newcap;
    }
}

/* helper: extend the capacity of a dynamic array of uint64_t */
static void extend_uint64_array(uint64_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t newcap = (*cap) ? (*cap) * 2 : 16;
        while (newcap < needed) newcap <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(*arr, newcap * sizeof(uint64_t));
        *arr = tmp;
        *cap = newcap;
    }
}

/* returns |true| if p divides any Lucas number */
static int divides_lucas(uint64_t p)
{
    if (p == 5ULL) return 0;          /* 5 never divides a Lucas number */

    uint64_t a = 2 % p;               /* L0 */
    uint64_t b = 1 % p;               /* L1 */
    if (a == 0) return 1;
    if (b == 0) return 1;

    /* Lucas numbers modulo p repeat with period dividing p - (5/p),
       hence it is sufficient to iterate up to p+1 steps. */
    for (uint64_t i = 2; i <= p + 1; ++i) {
        uint64_t c = a + b;
        if (c >= p) c -= p;
        if (c == 0) return 1;
        a = b;
        b = c;
    }
    return 0;
}

/* is candidate a prime?  Uses the list of already found primes. */
static int is_prime_small(uint64_t candidate,
                          uint64_t *prime_list, size_t prime_cnt)
{
    if (candidate < 2) return 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > candidate) break;
        if (candidate % p == 0) return 0;
    }
    return 1;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* static state: prime list, sequence cache, next candidate */
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt = 0, prime_cap = 0;

    static seq_t *seq_cache = NULL;
    static size_t seq_cnt = 0, seq_cap = 0;

    static uint64_t candidate = 2;          /* next integer to test */

    /* grow the cache until it contains at least n terms */
    while (seq_cnt < (size_t)n) {
        /* find next prime number */
        while (!is_prime_small(candidate, prime_list, prime_cnt))
            ++candidate;

        /* prime found: store it in the prime list */
        extend_uint64_array(&prime_list, &prime_cap, prime_cnt + 1);
        prime_list[prime_cnt++] = candidate;

        /* check divisibility */
        if (divides_lucas(candidate)) {
            extend_seq_array(&seq_cache, &seq_cap, seq_cnt + 1);
            seq_cache[seq_cnt++] = (seq_t)candidate;
        }

        ++candidate;
    }
    return seq_cache[(size_t)n - 1];
}
