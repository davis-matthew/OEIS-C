/*
 * A062565  Squarefree parts of 3-smooth numbers.
 * https://oeis.org/A062565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = squarefree part of the n‑th 3‑smooth number. The n‑th
 *            3‑smooth number is 2^i ⋅ 3^j, and a(n) = 2^(i mod 2) ⋅ 3^(j mod
 *            2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* static cache of exponent pairs for each 3‑smooth number */
    static unsigned char *i_arr = NULL;   /* exponent of 2 */
    static unsigned char *j_arr = NULL;   /* exponent of 3 */
    static idx_t        max_alloc = 0;     /* highest n computed so far */
    static idx_t        i2 = 1, i3 = 1;    /* indices used by the two‑pointer merge */

    /* initialise storage for the very first element (index 1) */
    if (max_alloc == 0) {
        i_arr = (unsigned char*)malloc(2 * sizeof(unsigned char));
        j_arr = (unsigned char*)malloc(2 * sizeof(unsigned char));
        if (!i_arr || !j_arr)
            return 0;                     /* allocation failure */
        i_arr[1] = 0;
        j_arr[1] = 0;
        max_alloc = 1;
    }

    /* if the caller requests a larger n, extend the cache */
    if (n > max_alloc) {
        unsigned char *new_i = (unsigned char*)realloc(i_arr,
                                                       (n + 1) * sizeof(unsigned char));
        unsigned char *new_j = (unsigned char*)realloc(j_arr,
                                                       (n + 1) * sizeof(unsigned char));
        if (!new_i || !new_j)
            return 0;                     /* allocation failure */
        i_arr = new_i;
        j_arr = new_j;

        /* constants used for comparison of 2^i⋅3^j values */
        const double ln2 = 0.6931471805599453094172321214581765680755;
        const double ln3 = 1.0986122886681096913952452369225257046474;
        const double eps = 1e-12;

        for (idx_t k = max_alloc + 1; k <= n; ++k) {
            double val2 = (i_arr[i2] + 1) * ln2 + j_arr[i2] * ln3;
            double val3 = i_arr[i3] * ln2 + (j_arr[i3] + 1) * ln3;

            if (val2 < val3 - eps) {                /* 2 * next candidate is smaller */
                i_arr[k] = i_arr[i2] + 1;
                j_arr[k] = j_arr[i2];
                ++i2;
            } else if (val3 < val2 - eps) {          /* 3 * next candidate is smaller */
                i_arr[k] = i_arr[i3];
                j_arr[k] = j_arr[i3] + 1;
                ++i3;
            } else {                                /* both give the same value */
                i_arr[k] = i_arr[i2] + 1;
                j_arr[k] = j_arr[i2];
                ++i2;
                ++i3;
            }
        }
        max_alloc = n;
    }

    /* compute the squarefree part from the stored exponents */
    seq_t result = 1;
    if (i_arr[n] & 1)
        result *= (seq_t)2;
    if (j_arr[n] & 1)
        result *= (seq_t)3;

    return result;
}
