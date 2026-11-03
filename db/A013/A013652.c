/*
 * A013652  Numbers k such that the period of the continued fraction for
 *          sqrt(k) contains at least three 1's.
 * https://oeis.org/A013652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers k such that the period of the continued fraction of
 *            sqrt(k) contains at least three 1's
 * Range:     u8 n <= 82, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9216 (2026-11-03)
 * Helpers:   isqrt_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Auxiliary functions for arbitrary‑precision seq_t.  */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isqrt_seq.h"
#include "../utils/mem.h"

/*  Count how many 1's occur in the periodic part of the continued
    fraction of sqrt(k).  Returns -1 if k is a perfect square.  */
static int count_ones_in_period(seq_t k)
{
    seq_t a0 = isqrt_seq(k);
    if (a0 * a0 == k)            /* perfect square */
        return -1;

    seq_t m   = 0;
    seq_t d   = 1;
    seq_t a   = a0;              /* a0 used to compute the first m, d */
    int   ones = 0;

    for (;;)
    {
        /* recurrence: m_{i+1} = d_i * a_i - m_i */
        m = d * a - m;

        /* d_{i+1} = (k - m_{i+1}^2) / d_i */
        seq_t diff = k - m * m;
        d = diff / d;

        /* a_{i+1} = floor((a0 + m_{i+1}) / d_{i+1}) */
        a = (a0 + m) / d;

        if (a == 1)
            ++ones;

        if (a == 2 * a0)      /* period ends */
            break;
    }

    return ones;
}

/*  Cache to store previously generated terms.  */
static seq_t *cache   = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

/*  Ensure the cache contains at least 'needed' slots.  */
static void cache_reserve(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp)
        exit(EXIT_FAILURE);   /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/*  Next integer to examine for the continued‑fraction property.  */
static seq_t next_k = 2;

/*  Main entry for the sequence.  */
seq_t a(idx_t n)
{
    if (n == 0)                     /* out of range, not used by OEIS */
        return 0;

    while (cache_size < (size_t)n)   /* generate up to n terms */
    {
        seq_t k;
        for (;;)
        {
            k = next_k++;
            /* skip perfect squares */
            if (count_ones_in_period(k) >= 3)
                break;
        }
        cache_reserve(cache_size + 1);
        cache[cache_size++] = k;
    }
    return cache[n - 1];
}
