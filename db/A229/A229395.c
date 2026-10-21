/*
 * A229395  Number of shapes of balanced 10-ary trees with n nodes, where a
 *          tree is balanced if the total number of nodes in subtrees
 *          corresponding to the branches of any node differ by at most one.
 * https://oeis.org/A229395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 13, u32 n <= 18, u64 n <= 33, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: let a(0)=1.
 * For n>0 set
 *   m = ⌊(n-1)/10⌋,  r = (n-1) mod 10
 *   a(n) = C(10,r) * a(m)^(10−r) * a(m+1)^r
 * where C(10,r) is the binomial coefficient.
 */
#include <stdlib.h>

static const seq_t BINOM10[11] = {
    (seq_t)1, (seq_t)10, (seq_t)45, (seq_t)120,
    (seq_t)210, (seq_t)252, (seq_t)210, (seq_t)120,
    (seq_t)45, (seq_t)10, (seq_t)1
};

seq_t a(idx_t n)
{
    /* Static cache for memoised values */
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;   /* allocated size of cache */
    static idx_t  last_computed = 0;/* index up to which values are already computed */

    /* Initialise cache[0] on first use */
    if (cache_size == 0) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cache[0] = (seq_t)1;
        cache_size = 1;
        last_computed = 0;
    }

    if (n == 0) return cache[0];

    /* Extend cache if needed */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute any missing values up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* Determine m and r */
        idx_t m = (i - 1) / 10;      /* floor((i-1)/10) */
        idx_t r = (i - 1) % 10;      /* (i-1) mod 10 */

        seq_t prod = 1;
        seq_t base_m = cache[m];

        /* Multiply a(m)^(10 - r) */
        for (idx_t k = 0; k < 10 - r; ++k) prod *= base_m;

        /* Multiply a(m+1)^r if necessary */
        if (r > 0) {
            seq_t base_m1 = cache[m + 1];
            for (idx_t k = 0; k < r; ++k) prod *= base_m1;
        }

        /* Multiply by the binomial coefficient */
        prod *= BINOM10[r];

        cache[i] = prod;
    }

    last_computed = n;
    return cache[n];
}
