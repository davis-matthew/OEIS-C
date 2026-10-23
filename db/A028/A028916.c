/*
 * A028916  Friedlander-Iwaniec primes: Primes of form a^2 + b^4.
 * https://oeis.org/A028916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 434, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 7402, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Friedlander–Iwaniec primes: primes of the form a² + b⁴.
   Computed by brute‑force search with caching. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include "../utils/is_prime.h"      /* bool is_prime(idx_t x); */

/* Integer square root for uint64_t using binary search. */
static uint64_t isqrt_uint64(uint64_t v)
{
    uint64_t lo = 0, hi = ((uint64_t)1 << 32);   /* sqrt(2^64) = 2^32 */
    while (lo < hi)
    {
        uint64_t mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == v) return mid;
        if (sq < v)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Test whether a prime p can be written as a² + b⁴. */
static bool is_of_form(uint64_t p)
{
    for (uint64_t b = 1; ; ++b)
    {
        unsigned __int128 b2 = (unsigned __int128)b * b;   /* b²   */
        unsigned __int128 b4 = b2 * b2;                    /* b⁴   */
        if (b4 > p) break;                                /* stop  */
        uint64_t rem = p - (uint64_t)b4;                  /* a²   */
        uint64_t a   = isqrt_uint64(rem);
        if (a * a == rem) return true;                    /* perfect square */
    }
    return false;
}

/* Required function: nth Friedlander‑Iwaniec prime (1‑based index). */
seq_t a(idx_t n)
{
    /* Cached sequence values. */
    static seq_t *seq   = NULL;
    static size_t seq_len = 0;  /* number of computed terms       */
    static size_t seq_cap = 0;  /* capacity of seq array           */
    static uint64_t next_candidate = 2; /* next integer to test         */

    /* Grow array if needed. */
    if (seq_len == seq_cap)
    {
        seq_cap = seq_cap ? seq_cap * 2 : 16;
        seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        if (!seq) abort(); /* out of memory */
    }

    /* Compute terms until we have at least n values. */
    while (seq_len < n)
    {
        uint64_t p = next_candidate++;
        /* Skip even numbers > 2. */
        if (p > 2 && (p & 1) == 0) continue;
        if (!is_prime((idx_t)p)) continue;          /* not prime */

        if (is_of_form(p))
        {
            seq[seq_len++] = (seq_t)p;              /* store the prime */
        }
    }
    return seq[n - 1];
}
