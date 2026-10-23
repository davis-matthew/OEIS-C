/*
 * A033270  Number of odd primes <= n.
 * https://oeis.org/A033270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of odd primes <= n (OEIS A033270)
 * Range:     u8 n <= 1620, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static storage for sieve data */
static uint8_t  *is_composite = (uint8_t *)0;   /* 0 = prime candidate, 1 = composite */
static seq_t   *prefix       = (seq_t *)0;   /* prefix[i] = number of odd primes <= i */
static idx_t    curr_limit   = 0;             /* highest n for which tables are built */

/* Rebuild sieve up to 'limit' (inclusive).  Caller guarantees limit >= 2. */
static void
recompute_sieve(idx_t limit)
{
    /* Allocate arrays with explicit casts for C++ compatibility */
    uint8_t *composite = (uint8_t *)calloc((size_t)(limit + 1), sizeof(uint8_t));
    seq_t   *pref      = (seq_t *)malloc((size_t)(limit + 1) * sizeof(seq_t));

    if (!composite || !pref) {
        free(composite);
        free(pref);
        return;   /* Allocation failed – leave old tables unchanged */
    }

    pref[0] = 0;
    pref[1] = 0;
    seq_t count = 0;

    /* Simple Eratosthenes sieve */
    for (idx_t i = 2; i <= limit; ++i) {
        if (!composite[i]) {                 /* i is prime */
            if (i > 2)                       /* count only odd primes */
                count += 1;
            /* Mark composites by stepping |i| rather than |i*i| to avoid overflow */
            for (idx_t j = i * 2; j <= limit; j += i)
                composite[j] = 1;
        }
        pref[i] = count;                      /* store prefix count */
    }

    /* Replace old tables with the new ones */
    free(is_composite);
    free(prefix);
    is_composite = composite;
    prefix       = pref;
    curr_limit   = limit;
}

/* Return the nth term of the sequence (a(1) == 0). */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                            /* a(1) = 0, a(0) undefined but harmless */

    if (n > curr_limit)                      /* tables too small, rebuild */
        recompute_sieve(n);

    /* If recomputation failed, return 0 as a safe fallback */
    if (!prefix)
        return 0;

    return prefix[n];
}
