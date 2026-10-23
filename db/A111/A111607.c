/*
 * A111607  Fourth column of A109626.
 * https://oeis.org/A111607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{⌊(n-1)/8⌋} (k+1) * p_{n-1-8k} where p =
 *            {1,2,3,3,5,3,7,2,7,6,5,3,3,1,1} (coefficients of the numerator
 *            of the generating function)
 * Range:     u8 n <= 256, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   /* the sequence is defined for n ≥ 1 only */
        return 0;

    /* coefficients of the numerator x*(1+2x+3x²+…+x¹⁴) */
    static const seq_t P[15] =
    { 1, 2, 3, 3, 5, 3, 7, 2, 7, 6, 5, 3, 3, 1, 1 };

    seq_t result = 0;
    idx_t max_k = (n - 1) / 8;          /* largest k with 8*k ≤ n-1 */

    for (idx_t k = 0; k <= max_k; ++k) {
        idx_t m = (n - 1) - 8 * k;      /* index into P, 0‑based */
        if (m <= 14)                    /* coefficient exists */
            result += ((seq_t)(k + 1)) * P[m];
    }
    return result;
}
