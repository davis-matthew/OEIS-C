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
 * Validated: standard, pass, 79 terms through n = 8928 (2026-11-03)
 * Helpers:   mem
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
#include "../utils/mem.h"

/* Return true iff k satisfies the condition for A069900. */
static bool ok_k(uint64_t k)
{
    if (k < 4)                     /* 4 is the smallest composite, but it fails the test */
        return false;

    /* Find the smallest prime factor p. */
    uint64_t p = 0;
    if ((k & 1) == 0)              /* even */
        p = 2;
    else
        for (uint64_t d = 3; d * d <= k; d += 2)
            if (k % d == 0) { p = d; break; }

    if (p == 0)                     /* k is prime */
        return false;

    /* Compute the largest prime factor q of k. */
    uint64_t q = 0;

    /* Remove all copies of the smallest prime factor to avoid re‑checking it. */
    uint64_t m = k;
    while (m % p == 0)
        m /= p;

    if (m == 1)                     /* k was a prime power */
        return false;

    uint64_t temp = m;
    for (uint64_t d = 2; d * d <= temp; d += (d == 2 ? 1 : 2))
    {
        if (temp % d == 0)
        {
            if (d > q) q = d;
            while (temp % d == 0) temp /= d;
        }
    }
    if (temp > 1 && temp > q)     /* remainder is a prime factor */
        q = temp;

    return q >= 2 * p;
}

/* Return the n-th term (1‑based) of sequence A069900. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, but safe */

    static seq_t *seq = NULL;              /* cache of previous terms */
    static size_t seq_len = 0;             /* number of terms cached */
    static uint64_t next_k = 2;            /* next integer to examine */

    /* Allocate initial cache space. */
    if (seq == NULL)
        seq = mem_new(seq_t, 64);

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
            seq = mem_resize(seq_t, seq, new_cap);
        }

        /* Store the term. */
        seq[seq_len++] = (seq_t)next_k;
        ++next_k;
    }

    /* Return the requested term (1‑based). */
    return seq[n - 1];
}
