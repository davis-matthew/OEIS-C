/*
 * A010093  Weight distribution of d=4 Hamming code of length 127.
 * https://oeis.org/A010093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = A010088(2n) Computes the weight distribution of the d=4
 *            Hamming code of length 127. The dual code has length 127 and
 *            dimension 7; this function counts codewords of weight 2n by
 *            enumerating all subsets of the 127 non‑zero 7‑bit vectors that
 *            XOR to zero. For n>63 the weight would exceed the...
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Return 0 for weights larger than 126 (i.e., n > 63). */
    if (n > 63u) {
        return (seq_t)0;
    }

    /* The DP table is built only once. */
    static bool built = false;
    static seq_t dp[128][128];

    if (!built) {
        /* Initialise the table. */
        for (int w = 0; w <= 127; ++w) {
            for (int s = 0; s < 128; ++s) {
                dp[w][s] = (seq_t)0;
            }
        }
        dp[0][0] = (seq_t)1;

        /* Enumerate the 127 non‑zero 7‑bit vectors (columns). */
        for (int v = 1; v <= 127; ++v) {
            /* Update DP from higher weight downwards. */
            for (int w = 127; w >= 1; --w) {
                for (int s = 0; s < 128; ++s) {
                    seq_t add = dp[w - 1][s];
                    if (add) {
                        int ns = s ^ v;
                        dp[w][ns] += add;
                    }
                }
            }
        }
        built = true;
    }

    /* Desired weight is 2*n. */
    int weight = (int)((seq_t)n * (seq_t)2);
    return dp[weight][0];
}
