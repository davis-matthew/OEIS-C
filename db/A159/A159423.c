/*
 * A159423  Number of n X n arrays of squares of integers summing to 29.
 * https://oeis.org/A159423
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^29 in (1 + x + x^4 + x^9 + x^16 +
 *            x^25)^(n^2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

typedef struct {
    seq_t a[30][30];
} matrix;

seq_t a(idx_t n)
{
    if (n < 2) return 0;                 /* outside offset */
    idx_t m = n * n;                     /* number of positions in the array */

    /* result vector: ways to obtain each sum 0..29 */
    seq_t vec[30], tv[30];
    for (int i = 0; i < 30; ++i) vec[i] = 0;
    vec[0] = 1;

    /* base transition matrix */
    static matrix baseM;
    static bool init = false;
    if (!init) {
        const int S[6] = {0, 1, 4, 9, 16, 25};
        for (int i = 0; i < 30; ++i)
            for (int j = 0; j < 30; ++j)
                baseM.a[i][j] = 0;
        for (int k = 0; k < 6; ++k)
            for (int t = S[k]; t < 30; ++t)
                baseM.a[t][t - S[k]] += 1;
        init = true;
    }

    matrix power = baseM;
    while (m > 0) {
        if (m & 1) {                     /* vec = vec * power */
            for (int t = 0; t < 30; ++t) {
                tv[t] = 0;
                for (int s = 0; s < 30; ++s) {
                    if (power.a[t][s] != 0)
                        tv[t] += vec[s] * power.a[t][s];
                }
            }
            for (int t = 0; t < 30; ++t) vec[t] = tv[t];
        }

        /* power = power * power */
        matrix tmp;
        for (int i = 0; i < 30; ++i) {
            for (int j = 0; j < 30; ++j) {
                seq_t sum = 0;
                for (int k = 0; k < 30; ++k) {
                    if (power.a[i][k] != 0 && power.a[k][j] != 0)
                        sum += power.a[i][k] * power.a[k][j];
                }
                tmp.a[i][j] = sum;
            }
        }
        for (int i = 0; i < 30; ++i)
            for (int j = 0; j < 30; ++j)
                power.a[i][j] = tmp.a[i][j];

        m >>= 1;
    }

    return vec[29];
}
