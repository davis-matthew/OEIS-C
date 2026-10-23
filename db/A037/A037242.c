/*
 * A037242  Molien series for 3-D group R2+R3.
 * https://oeis.org/A037242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of x^n in
 *            (1+x+2*x^2+4*x^3+8*x^4+4*x^5+2*x^6+x^7+x^8)/((1-x^2)^2*(1-x^3)^2)
 * Range:     u8 n <= 12, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* memoised values */
static idx_t cache_size = 0;  /* number of stored values */

/* coefficients of the numerator polynomial, 0 ≤ i ≤ 8 */
static const seq_t num_coeff[9] = {
    (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)4,
    (seq_t)8, (seq_t)4, (seq_t)2, (seq_t)1, (seq_t)1
};

seq_t a(idx_t n)
{
    if (cache_size <= n) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {                 /* allocation failure – safe fallback */
            return (seq_t)0;
        }
        cache = tmp;
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t val = 0;
            /* numerator contribution for i ≤ 8 */
            if (i <= 8)
                val += num_coeff[i];

            /* positive contributions from the denominator */
            if (i >= 2)
                val += ((seq_t)2) * cache[i - 2];
            if (i >= 3)
                val += ((seq_t)2) * cache[i - 3];
            if (i >= 7)
                val += ((seq_t)2) * cache[i - 7];
            if (i >= 8)
                val += ((seq_t)2) * cache[i - 8];

            /* negative contributions from the denominator */
            if (i >= 4)
                val -= cache[i - 4];
            if (i >= 5)
                val -= ((seq_t)4) * cache[i - 5];
            if (i >= 6)
                val -= cache[i - 6];
            if (i >= 10)
                val -= cache[i - 10];

            cache[i] = val;
        }
        cache_size = new_size;
    }
    return cache[n];
}
