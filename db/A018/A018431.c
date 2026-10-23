/*
 * A018431  Divisors of 396.
 * https://oeis.org/A018431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Divisors of 396 in increasing order */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* 396 = 2^2 * 3^2 * 11^1. Generate its 18 divisors once. */
    static seq_t divs[18];
    static int init = 0;
    if (!init) {
        size_t pos = 0;
        /* Powers of each prime factor. */
        const seq_t p2[3]  = { (seq_t)1, (seq_t)2, (seq_t)4 };   /* 2^0 .. 2^2 */
        const seq_t p3[3]  = { (seq_t)1, (seq_t)3, (seq_t)9 };   /* 3^0 .. 3^2 */
        const seq_t p11[2] = { (seq_t)1, (seq_t)11 };            /* 11^0 .. 11^1 */

        for (size_t i = 0; i < 3; ++i)
            for (size_t j = 0; j < 3; ++j)
                for (size_t k = 0; k < 2; ++k)
                    divs[pos++] = p2[i] * p3[j] * p11[k];

        /* Simple insertion sort for the 18 values. */
        for (size_t i = 1; i < 18; ++i) {
            seq_t key = divs[i];
            size_t j = i;
            while (j > 0 && divs[j-1] > key) {
                divs[j] = divs[j-1];
                --j;
            }
            divs[j] = key;
        }
        init = 1;
    }

    if (n < 1 || n > 18) return 0;
    return divs[(size_t)(n - 1)];
}
