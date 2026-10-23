/*
 * A136806  Nonsquares mod 65537.
 * https://oeis.org/A136806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 9775, u64 n <= 9775, u128 n <= 9775,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth quadratic nonresidue modulo 65537 (sequence A136806). */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mod_pow.h"

static seq_t *cache = NULL;
static size_t cache_size = 0;

/* Build the cache of all quadratic nonresidues modulo 65537. */
static void build_cache(void)
{
    if (cache != NULL)            /* already built */
        return;

    const seq_t MOD = (seq_t)65537;   /* Fermat prime */
    const uint64_t EXP = 32768;       /* (MOD-1)/2 */

    /* There are exactly (MOD-1)/2 = 32768 nonresidues. */
    cache_size = 32768;
    cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
    if (cache == NULL)
        return;                     /* allocation failure */

    size_t pos = 0;
    for (seq_t a = (seq_t)1; a < MOD; ++a) {
        /* Euler's criterion: a is a nonresidue iff a^EXP ≡ -1 (mod MOD). */
        if (mod_pow(a, EXP, MOD) == (MOD - (seq_t)1)) {
            cache[pos++] = a;
            if (pos == cache_size)
                break;
        }
    }
    /* pos should equal cache_size */
}

/* Return the nth element of the sequence (a(1)=3, a(2)=5, …). */
seq_t a(idx_t n)
{
    if (n <= (idx_t)0)
        return (seq_t)0;                 /* undefined for offset 0 */

    build_cache();

    if (!cache || n > (idx_t)cache_size)
        return (seq_t)0;                 /* out of bounds or allocation error */

    return cache[(size_t)n - 1];
}
