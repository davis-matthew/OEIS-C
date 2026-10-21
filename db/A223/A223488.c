/*
 * A223488  Number of distinct residues in the Lucas sequence mod the n-th
 *          prime.
 * https://oeis.org/A223488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes the number of distinct residues in the Lucas sequence
 * modulo the n-th prime.  The Lucas numbers are L0 = 2, L1 = 1,
 * Lk = Lk-1 + Lk-2 for k ≥ 2.  For a given prime p the set
 * { Lk mod p : k ≥ 0 } is periodic; the algorithm enumerates one
 * full period and counts the distinct residues.
 */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/get_prime.h"

/* cache for computed values: cache[n] holds a(n) for n ≥ 1. */
static seq_t *cache = NULL;
static unsigned char *cached = NULL;
static size_t cache_cap = 0;

/* ensure cache has space for indices up to n */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap)
        return;
    size_t new_cap = ((size_t)n + 1) * 2;
    cache  = (seq_t*)realloc(cache,  new_cap * sizeof(seq_t));
    cached = (unsigned char*)realloc(cached, new_cap * sizeof(unsigned char));
    /* initialise new slots to 0 */
    memset(cached + cache_cap, 0, (new_cap - cache_cap) * sizeof(unsigned char));
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    ensure_cache(n);
    if (cached[(size_t)n])
        return cache[(size_t)n];

    /* get the n-th prime */
    uint32_t p32 = get_prime(n);         /* 32‑bit prime value */
    uint64_t p   = (uint64_t)p32;        /* use 64‑bit for arithmetic */

    /* allocate a bit array for residues seen */
    unsigned char *seen = (unsigned char*)calloc(p, 1);
    if (!seen)
        return 0;          /* out of memory – should never happen */

    /* initialise Lucas values */
    uint64_t init0 = 2 % p;          /* L0 mod p */
    uint64_t init1 = 1 % p;          /* L1 mod p */
    uint64_t prev  = init0;
    uint64_t cur   = init1;

    size_t count = 0;
    if (!seen[prev]) { seen[prev] = 1; ++count; }
    if (!seen[cur])  { if (cur != prev) { seen[cur] = 1; ++count; } }

    /* iterate until the starting pair (init0, init1) recurs */
    for (;;)
    {
        uint64_t next = prev + cur;
        if (next >= p) next -= p;        /* fast modulo when sum < 2p */

        prev = cur;
        cur  = next;

        if (prev == init0 && cur == init1)
            break;                      /* period completed */

        if (!seen[cur])
        {
            seen[cur] = 1;
            ++count;
        }
    }

    free(seen);

    cache[(size_t)n] = (seq_t)count;
    cached[(size_t)n] = 1;
    return cache[(size_t)n];
}
