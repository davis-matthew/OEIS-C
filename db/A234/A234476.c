/*
 * A234476  Number of (n+1) X (1+1) 0..7 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 1
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 224*a(n-2) - 10304*a(n-4) + 96256*a(n-6) - 80640*a(n-8)
 *            * for n ≥ 9, with initial values * a(1)=672, a(2)=7840,
 *            a(3)=95872, a(4)=1200640, * a(5)=15217152, a(6)=194145280,
 *            a(7)=2484815872, * a(8)=31866634240 * * The function computes
 *            the sequence iteratively using a fixed‑size * circular...
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 15, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base terms a(1) … a(8) */
    static const seq_t base[8] = {
        (seq_t)672,
        (seq_t)7840,
        (seq_t)95872,
        (seq_t)1200640,
        (seq_t)15217152,
        (seq_t)194145280,
        (seq_t)2484815872,
        (seq_t)31866634240
    };

    if (n <= 8)
        return base[n - 1U];

    /* Buffer holds the last eight values:
     * buf[0] = a(k-8), buf[1] = a(k-7), ..., buf[7] = a(k-1)
     * When computing a(k) we need a(k-2)=buf[6], a(k-4)=buf[4],
     * a(k-6)=buf[2], a(k-8)=buf[0].
     */
    seq_t buf[8];
    for (size_t i = 0; i < 8; ++i)
        buf[i] = base[i];

    /* Compute subsequent terms up to n */
    for (idx_t k = 9; k <= n; ++k) {
        seq_t val = 224 * buf[6] - 10304 * buf[4]
                  + 96256 * buf[2] - 80640 * buf[0];

        /* Shift buffer left: drop oldest term (a(k-8)) */
        for (size_t i = 0; i < 7; ++i)
            buf[i] = buf[i + 1];
        buf[7] = val;
    }

    /* After the loop, buf[7] holds a(n) */
    return buf[7];
}
