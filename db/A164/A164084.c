/*
 * A164084  Number of reduced words of length n in Coxeter group on 39
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^6 = I.
 * https://oeis.org/A164084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 * a(1) = 39 * a(2) = 1482 * a(3) = 56316 * a(4) =
 *            2140008 * a(5) = 81320304 * a(6) =
 *            37·a(5)+37·a(4)+37·a(3)+37·a(2)+37·a(1)-703·a(0)+1 * a(n) =
 *            37·a(n-1)+37·a(n-2)+37·a(n-3)+37·a(n-4)+37·a(n-5)-703·a(n-6) (n
 *            ≥ 7)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* last six terms: a(n-6) .. a(n-1) before computing a(n) */
    seq_t prev[6] = {0};
    seq_t val = 0;

    for (idx_t i = 0; i <= n; ++i)
    {
        if (i == 0)
            val = 1;
        else if (i == 1)
            val = 39;
        else if (i == 2)
            val = 1482;
        else if (i == 3)
            val = 56316;
        else if (i == 4)
            val = 2140008;
        else if (i == 5)
            val = 81320304;
        else if (i == 6)
            val = 37 * (prev[5] + prev[4] + prev[3] + prev[2] + prev[1])
                  - 703 * prev[0] + 1;
        else  /* i >= 7 */
            val = 37 * (prev[5] + prev[4] + prev[3] + prev[2] + prev[1])
                  - 703 * prev[0];

        /* shift buffer: drop a(n-6), shift others left, store a(n) */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = prev[5];
        prev[5] = val;
    }

    return val;
}
