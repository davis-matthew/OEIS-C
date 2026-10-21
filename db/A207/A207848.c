/*
 * A207848  Number of 5 X n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 0 and 1 1 1 vertically.
 * https://oeis.org/A207848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-2) + 4*a(n-3) - 32*a(n-4) - 4*a(n-5) + 15*a(n-6) -
 *            a(n-8) for n > 9; initial values a(1)=16, a(2)=256, a(3)=704,
 *            a(4)=2816, a(5)=10144, a(6)=38208, a(7)=140032, a(8)=524480,
 *            a(9)=1928000
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 14, u64 n <= 31, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for computed values */
    static seq_t *cache = NULL;          /* cache[i] holds a(i) (cache[0] unused) */
    static idx_t cache_size = 0;         /* current allocated size of cache   */
    static idx_t computed_upto = 0;      /* highest n that has been computed  */

    /* initialise cache with the first nine values */
    if (cache_size == 0) {
        /* allocate room for at least first 10 indices (0..9) */
        cache_size = 16;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        /* zero the unused entry cache[0] */
        cache[0] = 0;
        /* store the known terms */
        cache[1] = (seq_t)16ULL;
        cache[2] = (seq_t)256ULL;
        cache[3] = (seq_t)704ULL;
        cache[4] = (seq_t)2816ULL;
        cache[5] = (seq_t)10144ULL;
        cache[6] = (seq_t)38208ULL;
        cache[7] = (seq_t)140032ULL;
        cache[8] = (seq_t)524480ULL;
        cache[9] = (seq_t)1928000ULL;
        computed_upto = 9;
    }

    /* ensure we have space for n */
    if (cache_size <= n) {
        idx_t new_size = cache_size * 2;
        if (new_size <= n) new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        cache = tmp;
        cache_size = new_size;
    }

    /* if requested value is already computed return it */
    if (n <= computed_upto) {
        return cache[n];
    }

    /* compute missing values up to n */
    for (idx_t k = computed_upto + 1; k <= n; ++k) {
        if (k <= 9) {
            /* base values already set */
            continue;
        }
        /* recurrence */
        seq_t term = cache[k-2] * ((seq_t)15);
        term += cache[k-3] * ((seq_t)4);
        term -= cache[k-4] * ((seq_t)32);
        term -= cache[k-5] * ((seq_t)4);
        term += cache[k-6] * ((seq_t)15);
        term -= cache[k-8];
        cache[k] = term;
    }

    computed_upto = n;
    return cache[n];
}
