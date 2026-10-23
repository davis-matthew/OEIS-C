/*
 * A109355  Numbers k such that the string 23572357k is prime.
 * https://oeis.org/A109355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 2322, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, push_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A109355: numbers k such that 23572357⋅10^d + k is prime,
 * with d = number of decimal digits of k. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/push_seq.h"

seq_t a(idx_t n)
{
    /* Ensure n is positive. */
    if (n == 0) return (seq_t)0;

    /* Static state to keep computed terms between calls. */
    static seq_t   *seq   = NULL;  /* array of found k's    */
    static idx_t    cap    = 0;    /* allocated capacity    */
    static idx_t    size   = 0;    /* number of terms stored*/

    /* Current candidate and helper for the 10^digits factor. */
    static uint64_t current_k = 1; /* next k to test          */
    static uint64_t pow10     = 10;/* 10^d for current k      */
    const uint64_t  BASE       = 23572357ULL;

    /* Generate terms until we have at least n terms. */
    while (size < n)
    {
        /* Concatenate: N = BASE*10^d + k */
        uint64_t N = BASE * pow10 + current_k;

        /* Test primality.  is_prime handles 64‑bit numbers. */
        if (is_prime(N))
            push_seq(&seq, &cap, &size, (seq_t)current_k);

        /* Advance candidate and update pow10 when crossing a power of 10. */
        current_k++;
        if (current_k == pow10)
            pow10 *= 10;
    }

    /* Return the (n-1)‑th stored term (offset 1). */
    return seq[n - 1];
}
