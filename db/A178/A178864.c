/*
 * A178864  Divisors of 27720.
 * https://oeis.org/A178864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, easy
 * Formula:   a(n) = the n-th divisor of 27720 in ascending order
 * Range:     u8 n <= 2, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The factorisation of 27720 is 2^3 * 3^2 * 5 * 7 * 11.
   There are 4*3*2*2*2 = 96 divisors.  This function
   returns the n-th divisor in sorted order for all n ≥ 1. */

#include <stdlib.h>

/* Comparator for qsort on seq_t values */
static int seq_compare(const void *p1, const void *p2)
{
    const seq_t a = *(const seq_t *)p1;
    const seq_t b = *(const seq_t *)p2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* 96 divisors of 27720 */
    static seq_t divs[96];
    static int init = 0;

    if (!init) {
        int i;
        int idx = 0;

        /* Precompute powers of each prime factor */
        seq_t pow2[4];
        seq_t pow3[3];
        seq_t pow5[2];
        seq_t pow7[2];
        seq_t pow11[2];

        pow2[0] = (seq_t)1;
        for (i = 1; i < 4; ++i) pow2[i] = pow2[i-1] * (seq_t)2;

        pow3[0] = (seq_t)1;
        for (i = 1; i < 3; ++i) pow3[i] = pow3[i-1] * (seq_t)3;

        pow5[0] = (seq_t)1;
        for (i = 1; i < 2; ++i) pow5[i] = pow5[i-1] * (seq_t)5;

        pow7[0] = (seq_t)1;
        for (i = 1; i < 2; ++i) pow7[i] = pow7[i-1] * (seq_t)7;

        pow11[0] = (seq_t)1;
        for (i = 1; i < 2; ++i) pow11[i] = pow11[i-1] * (seq_t)11;

        /* Build all 96 divisors */
        for (int e2 = 0; e2 < 4; ++e2)
            for (int e3 = 0; e3 < 3; ++e3)
                for (int e5 = 0; e5 < 2; ++e5)
                    for (int e7 = 0; e7 < 2; ++e7)
                        for (int e11 = 0; e11 < 2; ++e11) {
                            divs[idx++] = pow2[e2] * pow3[e3] * pow5[e5]
                                          * pow7[e7] * pow11[e11];
                        }

        /* Sort ascending */
        qsort(divs, 96, sizeof(seq_t), seq_compare);
        init = 1;
    }

    if (n < 1 || n > 96)
        return (seq_t)0;   /* undefined for outside the sequence */

    return divs[n - 1];
}
