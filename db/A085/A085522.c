/*
 * A085522  Product_{k=0..n} (3^(2k+1)+1).
 * https://oeis.org/A085522
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=0..n} (3^(2k+1)+1) (offset:–1, a(–1)=1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < -1) return (seq_t)0;          /* undefined below offset */

    /* static state for memoisation */
    static seq_t *product = NULL;          /* a(n) values, index = n+1 */
    static seq_t *power   = NULL;          /* 3^(2k+1) values, index = k+1 */
    static idx_t  max_n   = -2;            /* highest n computed so far */
    static idx_t  sz      = 0;             /* current allocation size   */

    /* Initialise on first call */
    if (!product) {
        sz = 1;                            /* only a(-1) stored */
        product = (seq_t*)malloc(sz * sizeof(seq_t));
        power   = (seq_t*)malloc(sz * sizeof(seq_t));
        if (!product || !power) return (seq_t)0; /* allocation failed */
        product[0] = (seq_t)1;            /* a(-1) */
        power[0]   = (seq_t)0;            /* dummy, not used */
        max_n = -1;
    }

    /* Grow to cover requested n */
    if (n > max_n) {
        idx_t new_sz = n + 2;              /* indices 0..n+1 are needed */
        seq_t *tmp;

        tmp = (seq_t*)realloc(product, new_sz * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        product = tmp;

        tmp = (seq_t*)realloc(power, new_sz * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        power = tmp;

        /* Compute new entries */
        for (idx_t i = max_n + 2; i <= n + 1; ++i) {
            seq_t p;                       /* 3^(2k+1), k = i-1 */
            if (i == 1)                     /* k = 0  -> 3^1 */
                p = (seq_t)3;
            else
                p = power[i-1] * (seq_t)9;   /* multiply previous by 9 */
            power[i] = p;
            seq_t term = p + (seq_t)1;      /* (3^(2k+1)+1) */
            product[i] = product[i-1] * term;
        }
        max_n = n;
        sz = new_sz;
    }

    return product[n + 1];
}
