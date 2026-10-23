/*
 * A161696  Number of reduced words of length n in the Weyl group B_3.
 * https://oeis.org/A161696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{j∈{0,2,4,8,10,12}} c_j * C(n-j+2, 2) with c =
 *            [1,-1,-1,1,1,-1]
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where C(k,2) = k*(k-1)/2  for k ≥ 2, otherwise 0. */
seq_t a(idx_t n)
{
    seq_t result = 0;
    static const int shifts[6] = {0, 2, 4, 8, 10, 12};
    static const int coeffs[6] = { 1, -1, -1,  1,  1, -1 };

    for (int i = 0; i < 6; ++i) {
        int j = shifts[i];
        if (n < (idx_t)j) continue;          /* term is zero for n< j */

        /* k = n - j + 2; use seq_t for multiplication */
        seq_t k = (seq_t)(n - (idx_t)j + 2);
        seq_t term = k * (k - 1) / 2;          /* binomial(k,2) */

        if (coeffs[i] > 0)
            result += term;
        else
            result -= term;
    }

    return result;
}
