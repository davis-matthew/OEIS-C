/*
 * A069900  Numbers k such that the integer quotient of largest and smallest
 *          prime factors of k is greater than one.
 * https://oeis.org/A069900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 156, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ok_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A069900: numbers k such that the integer quotient of the largest
 * prime factor of k by the smallest prime factor of k is greater than one.
 * Equivalently: k has a prime factor P ≥ 2 * p where p is its smallest
 * prime factor.
 *
 * The function generates these numbers on demand using a simple trial‑division
 * factorisation.  Results are cached so that repeated calls are fast.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ok_k.h"

/* Return the n-th term (1‑based) of sequence A069900. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, but safe */

    static seq_t *seq = NULL;              /* cache of previous terms */
    static size_t seq_len = 0;             /* number of terms cached */
    static uint64_t next_k = 2;            /* next integer to examine */

    /* Allocate initial cache space. */
    if (seq == NULL)
        seq = (seq_t *)malloc(64 * sizeof(seq_t));

    /* Generate terms until we have at least n of them. */
    while (seq_len < (size_t)n)
    {
        /* Find the next qualifying integer. */
        while (!ok_k(next_k))
            ++next_k;

        /* Grow the cache if necessary. */
        if (seq_len + 1 >= 64)
        {
            size_t new_cap = (seq_len + 1) * 2;
            seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        }

        /* Store the term. */
        seq[seq_len++] = (seq_t)next_k;
        ++next_k;
    }

    /* Return the requested term (1‑based). */
    return seq[n - 1];
}
