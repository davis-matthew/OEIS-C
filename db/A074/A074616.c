/*
 * A074616  a(n) = 5^n + 7^n.
 * https://oeis.org/A074616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5^n + 7^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 2, a(1) = 12, … (OEIS A074616) */

#include <stdlib.h>

/* compute a(n) by caching powers of 5 and 7 */
seq_t a(idx_t n)
{
    /* static cache for 5^k and 7^k */
    static seq_t *pow5 = NULL;         /* 5^i */
    static seq_t *pow7 = NULL;         /* 7^i */
    static idx_t pow_size = 0;         /* current allocated size */

    /* initialise cache for exponent 0 */
    if (pow_size == 0) {
        pow_size = 1;
        pow5 = (seq_t *)malloc(sizeof(seq_t));
        pow7 = (seq_t *)malloc(sizeof(seq_t));
        if (pow5 == NULL || pow7 == NULL)
            return (seq_t)0;   /* out of memory; 0 is a safe return in this context */
        pow5[0] = (seq_t)1;     /* 5^0 */
        pow7[0] = (seq_t)1;     /* 7^0 */
    }

    /* grow cache if needed */
    if ((idx_t)(n + 1) > pow_size) {
        idx_t new_size = (idx_t)(n + 1);
        seq_t *new5 = (seq_t *)realloc(pow5, new_size * sizeof(seq_t));
        seq_t *new7 = (seq_t *)realloc(pow7, new_size * sizeof(seq_t));
        if (new5 == NULL || new7 == NULL)
            return (seq_t)0;   /* out of memory; 0 is a safe return */
        pow5 = new5;
        pow7 = new7;
        for (idx_t i = pow_size; i < new_size; ++i) {
            pow5[i] = pow5[i - 1] * (seq_t)5;
            pow7[i] = pow7[i - 1] * (seq_t)7;
        }
        pow_size = new_size;
    }

    /* a(n) = 5^n + 7^n */
    return pow5[n] + pow7[n];
}
