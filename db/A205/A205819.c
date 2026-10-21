/*
 * A205819  Number of (n+1) X 5 0..2 arrays with the number of clockwise edge
 *          increases in every 2 X 2 subblock unequal to the number of
 *          counterclockwise edge increases.
 * https://oeis.org/A205819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) -18*a(n-2) -83*a(n-3) +264*a(n-4) +171*a(n-5)
 *            -1140*a(n-6) +234*a(n-7) +2180*a(n-8) -1280*a(n-9) -1912*a(n-10)
 *            +1667*a(n-11) +624*a(n-12) -873*a(n-13) +32*a(n-14) +162*a(n-15)
 *            -31*a(n-16) -8*a(n-17) +2*a(n-18); (a(1)=492, a(2)=2430, …,
 *            a(18)=2568038752491042)
 * Range:     u8 none, u16 n <= 3, u32 n <= 10, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static const idx_t init_len = 18;
    static const seq_t init[19] = {
        0ULL, /* unused index 0 */
        492ULL, 2430ULL, 12318ULL, 67944ULL, 380568ULL, 2158482ULL,
        12281976ULL, 70022628ULL, 399463842ULL, 2279603796ULL,
        13010464716ULL, 74259457206ULL, 423858388308ULL, 2419327070832ULL,
        13809259294038ULL, 78821939400948ULL, 449908555238976ULL,
        2568038752491042ULL
    };

    static seq_t *buf = NULL;
    static size_t buf_len = 0;

    /* Initialise the buffer with the known first 18 terms. */
    if (!buf) {
        buf_len = init_len + 1;
        buf = (seq_t *)malloc(buf_len * sizeof(seq_t));
        if (!buf) return 0ULL; /* allocation failure */
        for (size_t i = 0; i <= init_len; ++i)
            buf[i] = init[i];
    }

    if (n <= init_len)
        return buf[n];

    /* Extend the buffer if necessary. */
    if ((size_t)n + 1 > buf_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(buf, new_len * sizeof(seq_t));
        if (!tmp) return 0ULL; /* allocation failure */
        buf = tmp;
        /* Coefficients of the recurrence. */
        const int coeff[18] = {10, -18, -83, 264, 171, -1140, 234,
                               2180, -1280, -1912, 1667, 624, -873,
                               32, 162, -31, -8, 2};
        for (size_t i = buf_len; i <= (size_t)n; ++i) {
            seq_t pos = 0ULL;
            seq_t neg = 0ULL;
            /* compute positive and negative contributions separately */
            for (int k = 0; k < 18; ++k) {
                size_t idx = i - (k + 1);       /* previous term index */
                if (coeff[k] > 0) {
                    pos = pos + ((seq_t)coeff[k]) * buf[idx];
                } else {
                    neg = neg + ((seq_t)(-coeff[k])) * buf[idx];
                }
            }
            buf[i] = pos - neg; /* pos >= neg for all n */
        }
        buf_len = new_len;
    }

    return buf[n];
}
