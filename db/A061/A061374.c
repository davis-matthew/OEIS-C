/*
 * A061374  Numbers which have more different digits than their cubes.
 * https://oeis.org/A061374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   big_pow, digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th integer that has more distinct decimal digits than its cube.
 * This is OEIS A061374.  The function searches successive integers and
 * keeps those where the count of distinct digits in the number is strictly
 * greater than the count of distinct digits in its cube.  The result is
 * cached so that repeated calls are fast. */

#include <stdlib.h>
#include <stdbool.h>

/* Utility functions for digit frequency and large powers. */
#include "../utils/digit_frequencies.h"
#include "../utils/big_pow.h"

/* Static cache for the sequence values. */
static seq_t *seq_cache   = NULL;   /* array of found elements */
static size_t seq_len     = 0;      /* number of elements stored */
static size_t seq_cap     = 0;      /* allocated capacity of seq_cache */
static idx_t  next_candidate = 1;   /* next integer to test */

/* Count distinct decimal digits of a seq_t value. */
static unsigned int distinct_digits(seq_t x)
{
    unsigned int freq[10];
    digit_frequencies(x, freq);
    unsigned int count = 0;
    for (int d = 0; d < 10; ++d)
        if (freq[d]) ++count;
    return count;
}

/* Compute k^3 in a way that works for both built‑in integer seq_t and
 * arbitrary‑precision seq_t. */
static seq_t cube_seq_t(seq_t k)
{
    /* If seq_t is a built‑in unsigned integer, use a 128‑bit temporary
     * to avoid overflow (when the compiler supports __int128). */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
#if defined(__INT128__) || defined(__INT128)
        unsigned __int128 tmp = (unsigned __int128)k * k * k;
        return (seq_t)tmp;
#else
        return k * k * k;
#endif
    }
    /* Otherwise seq_t is an arbitrary‑precision type.  Use big_pow. */
    return big_pow(k, (seq_t)3);
}

/* Public entry point – the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* Undefined for offset, return 0 */

    /* Generate terms until we have at least n of them. */
    while (seq_len < n) {
        seq_t k = (seq_t)next_candidate;

        unsigned int distinct_k   = distinct_digits(k);
        seq_t cube                 = cube_seq_t(k);
        unsigned int distinct_cube = distinct_digits(cube);

        if (distinct_k > distinct_cube) {
            /* Store the qualifying number. */
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 16;
                seq_cache = (seq_t *)realloc(seq_cache,
                                             seq_cap * sizeof(seq_t));
            }
            seq_cache[seq_len++] = k;
        }
        ++next_candidate;
    }

    return seq_cache[n - 1];
}
