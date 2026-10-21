/*
 * A095317  Primes in whose binary expansion the number of 1 bits is <= number
 *          of 0 bits minus 2.
 * https://oeis.org/A095317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 932, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bitlen_seq_t, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes with (#1 bits) ≤ (#0 bits) – 2 in binary */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/bitlen_seq_t.h"

static unsigned popcount_seq(seq_t x)
{
    /* Fast popcount for ≤ 64‑bit values */
    if (sizeof(seq_t) <= sizeof(uint64_t))
        return __builtin_popcountll((uint64_t)x);

    /* Generic popcount using the “bitwise clean‑up” trick */
    seq_t t = x;
    unsigned cnt = 0;
    while (t) {
        t &= t - 1;
        ++cnt;
    }
    return cnt;
}

static void grow_cache(size_t need, size_t *cap, seq_t **arr)
{
    if (*cap < need) {
        size_t newcap = (*cap > 0) ? (*cap << 1) : 4;
        while (newcap < need) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        *arr = tmp;
        *cap = newcap;
    }
}

seq_t a(idx_t n)
{
    /* Static memoized cache of found terms */
    static seq_t *cache   = NULL;
    static size_t  clen   = 0;
    static size_t  ccap   = 0;
    static seq_t next     = 3;          /* first odd prime candidate */

    if (n == 0) return 0;  /* undefined for n = 0, but keep safe */

    /* grow cache until we have at least n elements */
    while (clen < (size_t)n) {
        seq_t cand = next;

        /* primality test suitable for the size of seq_t */
        if (!is_prime_big(cand)) {
            next += 2;
            continue;
        }

        unsigned ones = popcount_seq(cand);
        unsigned bits = bitlen_seq_t(cand);

        /* Condition: ones ≤ zeros – 2  <=>  bits ≥ 2*ones + 2 */
        if (bits >= 2u * ones + 2u) {
            grow_cache(clen + 1, &ccap, &cache);
            cache[clen++] = cand;
        }
        next += 2;  /* skip even numbers only after 2 */
    }
    return cache[n - 1];
}
