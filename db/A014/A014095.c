/*
 * A014095  Molien series for real extraspecial group 2^{1+2*3} of degree 8
 *          and order 128 formed from tensor products of Pauli matrices (0,1,
 *          1,0) and (1,0, 0,-1).
 * https://oeis.org/A014095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) is defined by a(0)=1, a(1)=1, a(2)=15, a(3)=29, a(4)=135,
 *            a(5)=310, a(6)=870, a(7)=1830, a(8)=3993, a(9)=7535,
 *            a(10)=14157, a(11)=24427, and for n≥12 a(n)= 4*a(n-1) - 2*a(n-2)
 *            -12*a(n-3) +17*a(n-4) + 8*a(n-5) -28*a(n-6) + 8*a(n-7)
 *            +17*a(n-8) -12*a(n-9) -2*a(n-10) +4*a(n-11) - a(n-12)
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 70, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* initial values of the sequence */
static const seq_t init[12] = {
    (seq_t)1, (seq_t)1, (seq_t)15, (seq_t)29, (seq_t)135,
    (seq_t)310, (seq_t)870, (seq_t)1830, (seq_t)3993,
    (seq_t)7535, (seq_t)14157, (seq_t)24427
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* extend cache so that it contains entry for n */
    if (n + (idx_t)1 > cache_size) {
        idx_t new_size = n + (idx_t)1;
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        }
        /* fill new entries if we have not yet initialised them */
        if (cache_size == 0) {
            for (idx_t i = 0; i < 12 && i < new_size; ++i) {
                cache[i] = init[i];
            }
            cache_size = (new_size < 12) ? new_size : 12;
        }
        /* compute values for indices ≥ cache_size */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i < 12) {
                cache[i] = init[i];
            } else {
                seq_t next = (seq_t)0;
                /* coefficients of the recurrence */
                const int coeffs[12] = {4, -2, -12, 17, 8, -28, 8, 17, -12, -2, 4, -1};
                for (int k = 0; k < 12; ++k) {
                    seq_t val = cache[i - 1 - k];
                    int coeff = coeffs[k];
                    if (coeff > 0) {
                        next += val * (seq_t)coeff;
                    } else {
                        seq_t temp = val * (seq_t)(-coeff);
                        next -= temp;
                    }
                }
                cache[i] = next;
                cache_size = i + 1;          /* keep track of the filled length */
            }
        }
    }

    return cache[n];
}
