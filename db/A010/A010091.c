/*
 * A010091  Weight distribution of d=4 Hamming code of length 31.
 * https://oeis.org/A010091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = A010086(2n) (weight distribution of the (31,26) Hamming
 *            code)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Compute the number of codewords of weight 2n in the binary Hamming code
   [31,26,3].  The weight distribution of the dual (simplex) code has
   5-bit columns 1..31.  We enumerate all subsets of columns whose XOR
   equals zero, counting their sizes.  This is a static one‑time
   pre‑computation.  The codeword weight is the subset size. */
seq_t a(idx_t n)
{
    /* 5‑bit masks, 31 columns */
    static int ready = 0;
    static seq_t weight[32];

    if (!ready) {
        /* dp[mask][w] = number of subsets of processed columns
           with XOR = mask and size = w.  Only 5 bits needed for mask. */
        seq_t dp[32][32] = {0};
        dp[0][0] = 1;               /* empty set */

        for (int i = 0; i < 31; ++i) {
            seq_t ndp[32][32] = {0};
            int col = i + 1;        /* 5‑bit column vector */

            for (int mask = 0; mask < 32; ++mask) {
                for (int w = 0; w <= i; ++w) {
                    seq_t cur = dp[mask][w];
                    if (cur == 0) continue;

                    /* exclude the current column */
                    ndp[mask][w] += cur;

                    /* include the current column */
                    ndp[mask ^ col][w + 1] += cur;
                }
            }

            /* copy ndp back */
            for (int mask = 0; mask < 32; ++mask)
                for (int w = 0; w <= i + 1; ++w)
                    dp[mask][w] = ndp[mask][w];
        }

        /* collect weights of codewords (mask 0 means valid codeword) */
        for (int w = 0; w <= 31; ++w)
            weight[w] = dp[0][w];

        ready = 1;
    }

    /* a(n) counts codewords of weight 2n.  If 2n > 31, return 0. */
    idx_t double_n = n << 1;  /* compute 2*n safely in idx_t */
    if (double_n > 31U)
        return (seq_t)0;
    return weight[(int)double_n];
}
