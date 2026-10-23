/*
 * A134950  Divisors of 453060.
 * https://oeis.org/A134950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, easy
 * Formula:   a(n) = the n-th divisor of 453060 in ascending order
 * Range:     u8 n <= 9, u16 n <= 32, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function generates all 48 divisors of 453060, sorts them,
   and returns the n-th one (1‑based).  For n>48 the value 0 is returned. */

#include <stdint.h>

seq_t a(idx_t n) {
    /* static storage for divisors and initialization flag */
    static seq_t divisors[48];
    static int initialized = 0;

    if (!initialized) {
        /* prime powers */
        seq_t p2[3] = {(seq_t)1, (seq_t)2, (seq_t)4};          /* 2^0 .. 2^2 */
        seq_t p3[4] = {(seq_t)1, (seq_t)3, (seq_t)9, (seq_t)27}; /* 3^0 .. 3^3 */
        seq_t p5[2] = {(seq_t)1, (seq_t)5};
        seq_t p839[2] = {(seq_t)1, (seq_t)839};

        /* generate all divisors */
        int idx = 0;
        for (int a = 0; a < 3; ++a) {          /* exponent of 2 */
            seq_t va = p2[a];
            for (int b = 0; b < 4; ++b) {      /* exponent of 3 */
                seq_t vb = va * p3[b];
                for (int c = 0; c < 2; ++c) {  /* exponent of 5 */
                    seq_t vc = vb * p5[c];
                    for (int d = 0; d < 2; ++d) { /* exponent of 839 */
                        divisors[idx++] = vc * p839[d];
                    }
                }
            }
        }

        /* insertion sort (size is only 48) */
        for (int i = 1; i < 48; ++i) {
            seq_t key = divisors[i];
            int j = i - 1;
            while (j >= 0 && divisors[j] > key) {
                divisors[j + 1] = divisors[j];
                --j;
            }
            divisors[j + 1] = key;
        }

        initialized = 1;
    }

    if (n >= 1 && n <= 48)
        return divisors[n - 1];
    else
        return (seq_t)0;
}
