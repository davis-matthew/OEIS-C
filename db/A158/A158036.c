/*
 * A158036  Integer solutions f for f = (4^n - 2^n + 8n^2 - 2) / (2n * (2n +
 *          1)) with n an integer.
 * https://oeis.org/A158036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = integer f for f = (4^k - 2^k + 8k^2 - 2)/(2k*(2k+1))
 *            where k ranges over the integers and only values of f that are
 *            integers are listed in increasing order. a(1) = 3, a(2) = 8287,
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility for growing a dynamic array of seq_t values */
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* keep a cache of previously computed a(m) values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;

    /* keep the current power values and the next integer k to test */
    static seq_t pow4 = (seq_t)1;     /* 4^0 */
    static seq_t pow2 = (seq_t)1;     /* 2^0 */
    static idx_t next_k = 1;          /* k to test next */

    /* if we already have the requested term, return it */
    if (n <= cache_size)
        return cache[n - 1];

    /* grow cache to at least one more element (per call) */
    if (cache_size == cache_cap)
        grow_seq(&cache, &cache_cap, cache_size + 1);

    /* search for terms until we have enough */
    while (cache_size < n) {
        /* update powers for current k */
        pow2 = pow2 * ((seq_t)2);
        pow4 = pow4 * ((seq_t)4);

        /* k as seq_t */
        seq_t k_seq = (seq_t)next_k;

        /* compute 8k^2 - 2 */
        seq_t k_sq = k_seq * k_seq;
        seq_t term = k_sq * ((seq_t)8);
        term = term - ((seq_t)2);

        /* numerator = 4^k - 2^k + (8k^2 - 2) */
        seq_t numerator = pow4 - pow2 + term;

        /* denominator = 2k * (2k + 1) */
        seq_t twok = k_seq * ((seq_t)2);
        seq_t denom = twok * (twok + ((seq_t)1));

        /* check if numerator divisible by denom */
        if ((numerator % denom) == ((seq_t)0)) {
            /* integer solution f */
            seq_t f = numerator / denom;
            cache[cache_size++] = f;
        }

        next_k++;
    }

    return cache[n - 1];
}
