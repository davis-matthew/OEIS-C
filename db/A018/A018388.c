/*
 * A018388  Divisors of 315.
 * https://oeis.org/A018388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 315: a(n) equals the n‑th divisor (1‑indexed) when the divisors
   of 315 are listed in increasing order. 
   The sequence has length 12 (1,3,5,7,9,15,21,35,45,63,105,315).*/

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Empty if called with n=0 – no term defined before the offset. */
    if (n == 0) return 0;

    /* The divisor set is static; compute once. */
    static seq_t divisors[12];
    static int init = 0;

    if (!init) {
        /* 3^0..3^2, 5^0..5^1, 7^0..7^1 */
        seq_t p3[3], p5[2], p7[2];
        p3[0] = 1;  p3[1] = 3;  p3[2] = 9;
        p5[0] = 1;  p5[1] = 5;
        p7[0] = 1;  p7[1] = 7;

        size_t idx = 0;
        seq_t val;
        for (int e3 = 0; e3 < 3; ++e3)
            for (int e5 = 0; e5 < 2; ++e5)
                for (int e7 = 0; e7 < 2; ++e7) {
                    val = p3[e3];
                    val = val * p5[e5];
                    val = val * p7[e7];
                    divisors[idx++] = val;
                }

        /* Sort in ascending order – simple bubble sort. */
        for (size_t i = 0; i < 12; ++i)
            for (size_t j = i + 1; j < 12; ++j)
                if (divisors[i] > divisors[j]) {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }

        init = 1;
    }

    /* Sequence has only 12 terms; indices beyond that return 0. */
    if (n > 12) return 0;

    /* n is 1‑based; convert to 0‑based array index. */
    return divisors[(size_t)n - 1];
}
