/*
 * A055341  Number of mobiles (circular rooted trees) with n nodes and 3
 *          leaves.
 * https://oeis.org/A055341
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - a(n-3) + a(n-5) + 2*a(n-6) -
 *            3*a(n-7) + a(n-8) Initial terms (offset 4): a(4)=1, a(5)=3,
 *            a(6)=8, a(7)=19, a(8)=37
 * Range:     u8 n <= 11, u16 n <= 43, u32 n <= 675, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* dynamic array of already computed terms   */
static idx_t  cache_size = 0;        /* size of the allocated array              */
static idx_t  computed_upto = 0;     /* largest index that has been computed     */

static void ensure_capacity(idx_t needed)
{
    if (cache_size <= needed) {
        idx_t old_size = cache_size;
        idx_t new_size = needed + 1;          /* indices 0 … needed          */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) exit(1);              /* out‑of‑memory abort         */
        cache = new_cache;
        cache_size = new_size;
        /* zero‑initialize the newly allocated part */
        for (idx_t i = old_size; i < new_size; ++i) cache[i] = 0;
    }
}

seq_t a(idx_t n)
{
    if (n < 4) return 0;                    /* behaviour below offset is undefined */

    /* make sure storage is large enough */
    ensure_capacity(n);

    /* compute any new entries */
    if (computed_upto < n) {
        idx_t start = (computed_upto < 4) ? 4 : computed_upto + 1;
        for (idx_t i = start; i <= n; ++i) {
            if (i == 4)          cache[4] = (seq_t)1;
            else if (i == 5)     cache[5] = (seq_t)3;
            else if (i == 6)     cache[6] = (seq_t)8;
            else if (i == 7)     cache[7] = (seq_t)19;
            else if (i == 8)     cache[8] = (seq_t)37;
            else
                cache[i] = 3*cache[i-1]
                           - 2*cache[i-2]
                           - cache[i-3]
                           + cache[i-5]
                           + 2*cache[i-6]
                           - 3*cache[i-7]
                           + cache[i-8];
        }
        computed_upto = n;
    }
    return cache[n];
}
