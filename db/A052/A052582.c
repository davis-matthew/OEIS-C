/*
 * A052582  a(n) = 2*n*n!.
 * https://oeis.org/A052582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*n*n!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
/* static array to cache factorials */
static seq_t *fac = NULL;
static size_t fac_cap = 0;
static size_t fac_used = 0;

seq_t a(idx_t n) {
    /* Ensure factorials up to n are computed */
    if (n >= fac_used) {
        if (fac == NULL) {
            /* initial allocation */
            fac_cap = 16;
            fac_used = 1;
            fac = (seq_t *)malloc(fac_cap * sizeof(seq_t));
            fac[0] = (seq_t)1;            /* 0! = 1 */
        }
        size_t new_used = n + 1;
        if (new_used > fac_cap) {
            /* grow array */
            size_t new_cap = fac_cap * 2;
            if (new_cap < new_used) new_cap = new_used;
            fac = (seq_t *)realloc(fac, new_cap * sizeof(seq_t));
            fac_cap = new_cap;
        }
        /* compute missing factorials */
        for (size_t i = fac_used; i < new_used; ++i)
            fac[i] = fac[i - 1] * (seq_t)(i);
        fac_used = new_used;
    }
    /* a(n) = 2 * n * n!  */
    return (seq_t)2 * (seq_t)n * fac[n];
}
