/*
 * A165179  Number of reduced words of length n in Coxeter group on 47
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^8 = I.
 * https://oeis.org/A165179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A165179: a(n) = 45*(a(n-1)+...+a(n-7)) – 1035*a(n-8)  for n ≥ 9
   with initial values
   a(0)=1, a(1)=47, a(2)=2162, a(3)=99452, a(4)=4574792,
   a(5)=210440432, a(6)=9680259872, a(7)=445291954112, a(8)=20483429888071. */

#include <stdlib.h>   /* calloc, realloc, abort */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache          = NULL;  /* array of seq_t values   */
    static idx_t cache_len       = 0;      /* number of allocated slots */
    static idx_t computed_up_to  = 0;      /* largest index already computed */

    /* constant initial terms */
    static const seq_t init_vals[9] = {
        (seq_t)1ULL,
        (seq_t)47ULL,
        (seq_t)2162ULL,
        (seq_t)99452ULL,
        (seq_t)4574792ULL,
        (seq_t)210440432ULL,
        (seq_t)9680259872ULL,
        (seq_t)445291954112ULL,
        (seq_t)20483429888071ULL   /* a(8) */
    };

    /* allocate and initialise cache once */
    if (cache == NULL) {
        size_t sz = (size_t)((n < 8) ? 9 : (idx_t)n + 1);
        cache = (seq_t *)calloc(sz, sizeof(seq_t));
        if (!cache) abort(); /* allocation failure */

        cache_len = (idx_t)sz;
        /* initialise the first nine terms */
        for (idx_t i = 0; i < 9; ++i)
            cache[i] = init_vals[i];

        computed_up_to = 8; /* a(8) is already known */
    }

    /* grow cache if needed */
    if ((idx_t)(n + 1) > cache_len) {
        idx_t new_len = (idx_t)(n + 1);
        if (new_len < 9) new_len = 9;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache      = tmp;
        cache_len  = new_len;
        /* new entries are zero; we will compute them below */
    }

    /* compute any missing terms up to n */
    if (n > computed_up_to) {
        /* start from computed_up_to+1 (which is at least 9) */
        for (idx_t i = computed_up_to + 1; i <= n; ++i) {
            seq_t sum = 0;
            /* 45 * (a(i-1) + ... + a(i-7)) */
            sum += (seq_t)45 * cache[i - 1];
            sum += (seq_t)45 * cache[i - 2];
            sum += (seq_t)45 * cache[i - 3];
            sum += (seq_t)45 * cache[i - 4];
            sum += (seq_t)45 * cache[i - 5];
            sum += (seq_t)45 * cache[i - 6];
            sum += (seq_t)45 * cache[i - 7];
            /* – 1035 * a(i-8) */
            sum -= (seq_t)1035 * cache[i - 8];
            cache[i] = sum;
        }
        computed_up_to = n;
    }

    return cache[n];
}
