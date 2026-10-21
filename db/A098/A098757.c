/*
 * A098757  Smallest available integer which fits into the repeating pattern
 *          02468.
 * https://oeis.org/A098757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the smallest unused integer whose decimal representation
 * uses only the digits {0,2,4,6,8} and does not end in 8.
 * Recurrence (for n ≥ 20):
 *   a(n) = a(n-16) * 10^5 + P
 *   where P is the cyclic permutation of 24680 whose last digit equals
 *   the last digit of a(n-16).  The four allowed values for P are:
 *     24680 (last digit 0), 46802 (2), 68024 (4), 80246 (6).
 *
 * Initial values:
 *   a(0)=0, a(1)=2, a(2)=4, a(3)=6, a(4)=80, a(5)=24, a(6)=680,
 *   a(7)=246, a(8)=802, a(9)=46, a(10)=8024, a(11)=6802, a(12)=4680,
 *   a(13)=24680, a(14)=246802, a(15)=46802, a(16)=468024, a(17)=68024,
 *   a(18)=680246, a(19)=80246.
 */

#include <stdlib.h>

static const seq_t init[20] = {
    (seq_t)0,     (seq_t)2,    (seq_t)4,     (seq_t)6,
    (seq_t)80,    (seq_t)24,   (seq_t)680,   (seq_t)246,
    (seq_t)802,   (seq_t)46,   (seq_t)8024,  (seq_t)6802,
    (seq_t)4680,  (seq_t)24680, (seq_t)246802,(seq_t)46802,
    (seq_t)468024,(seq_t)68024,(seq_t)680246,(seq_t)80246
};

static seq_t *cache = NULL;      /* cache[0..max_computed] */
static idx_t  cache_max = 0;     /* highest index currently cached */

/*
 * Helper: compute the next term for indices ≥20.
 */
static seq_t compute_term(idx_t n)
{
    /* n >= 20 */
    idx_t base_idx = n - 16;
    seq_t base = (base_idx < 20) ? init[base_idx] : cache[base_idx];
    int last = (int)( (unsigned long long)(base % (seq_t)10) );   /* 0,2,4,6 */

    seq_t block;
    switch (last) {
        case 0: block = (seq_t)24680; break;   /* ends in 0 */
        case 2: block = (seq_t)46802; break;   /* ends in 2 */
        case 4: block = (seq_t)68024; break;   /* ends in 4 */
        case 6: block = (seq_t)80246; break;   /* ends in 6 */
        default: block = (seq_t)0;             /* never reached */
    }
    return base * (seq_t)100000 + block;
}

/*
 * Main sequence function.
 */
seq_t a(idx_t n)
{
    if (n < 20)
        return init[(int)n];

    /* ensure cache is large enough */
    if (cache_max <= n) {
        idx_t new_max = n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_max * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failure; fall back to recomputing on the fly */
            cache = NULL;
            cache_max = 0;
        } else {
            cache = new_cache;
            /* fill initial known values */
            for (idx_t i = 0; i < 20; ++i)
                cache[i] = init[i];
            /* compute missing entries up to n */
            for (idx_t i = (cache_max < 20 ? 20 : cache_max); i <= n; ++i)
                cache[i] = compute_term(i);
            cache_max = new_max;
        }
    }

    /* if cache is available, return from it */
    if (cache_max > n && cache) {
        return cache[n];
    }

    /* otherwise compute on the fly (unlikely) */
    return compute_term(n);
}
