/*
 * A219916  Number of 3 X n arrays of the minimum value of corresponding
 *          elements and their horizontal, diagonal, diagonal or antidiagonal
 *          neighbors in a random, but sorted with lexicographically
 *          nondecreasing rows and nonincreasing columns, 0..2 3 X n array.
 * https://oeis.org/A219916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 10*a(n‑1) - 45*a(n‑2) + 120*a(n‑3) - 210*a(n‑4) + *
 *            252*a(n‑5) - 210*a(n‑6) + 120*a(n‑7) - 45*a(n‑8) + * 10*a(n‑9) -
 *            a(n‑10) for n > 15 * Initial values: * a(1) = 6, a(2) = 7, a(3)
 *            = 24, a(4) = 64, a(5) = 147, a(6) = 308, * a(7) = 606, a(8) =
 *            1144, a(9) = 2097, a(10) = 3761, * a(11) = 66...
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 52, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* External linkage function as required */
seq_t a(idx_t n)
{
    /* Return 0 for undefined n < 1 */
    if (n < 1)
        return 0;

    /* Initial terms a(1)..a(15) */
    static const seq_t init[15] = {
        (seq_t)6,   (seq_t)7,   (seq_t)24,  (seq_t)64,  (seq_t)147,
        (seq_t)308, (seq_t)606, (seq_t)1144, (seq_t)2097, (seq_t)3761,
        (seq_t)6628,(seq_t)11503, (seq_t)19682, (seq_t)33216, (seq_t)55293
    };

    /* Handle the first 15 terms directly */
    if (n <= 15)
        return init[n - 1];

    /* Ring buffer holding the last 10 values: a(i‑10) .. a(i‑1) */
    seq_t buf[10];
    /* Initialize buffer with a(6) .. a(15) */
    for (int k = 0; k < 10; ++k)
        buf[k] = init[5 + k];           /* init[5] = a(6) */

    seq_t val;  /* Value of a(i) for the current iteration */

    for (idx_t i = 16; i <= n; ++i)
    {
        val =
            (seq_t)10 * buf[9]   /* a(i-1) */
          - (seq_t)45 * buf[8]   /* a(i-2) */
          + (seq_t)120 * buf[7]  /* a(i-3) */
          - (seq_t)210 * buf[6]  /* a(i-4) */
          + (seq_t)252 * buf[5]  /* a(i-5) */
          - (seq_t)210 * buf[4]  /* a(i-6) */
          + (seq_t)120 * buf[3]  /* a(i-7) */
          - (seq_t)45 * buf[2]   /* a(i-8) */
          + (seq_t)10 * buf[1]   /* a(i-9) */
          - buf[0];              /* a(i-10) */

        /* Rotate buffer to discard a(i-10) and add a(i) */
        for (int k = 0; k < 9; ++k)
            buf[k] = buf[k + 1];
        buf[9] = val;
    }

    return val;
}
