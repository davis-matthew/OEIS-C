/*
 * A171907  a(n)= Number of 5 X 5 permutation matrices with trace n and
 *          determinant +1.
 * https://oeis.org/A171907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = C(5,n) * E(5-n) * where E(m) is the number of even
 *            derangements of m elements. * This counts even permutations
 *            (determinant +1) of 5×5 permutation * matrices with exactly n
 *            fixed points (trace n)
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Only values for n = 0..5 are non‑zero. */
    if (n > 5)
        return 0;

    /* Even derangement counts for m = 0..5.
     *  E(0) = 1 (empty permutation is even)
     *  E(1) = 0
     *  E(2) = 0
     *  E(3) = 2  (two 3‑cycles, both even)
     *  E(4) = 3  (three double‑transposition derangements)
     *  E(5) = 24 (24 even derangements of 5 elements) */
    static const seq_t E[6] = { 1, 0, 0, 2, 3, 24 };

    /* Compute binomial coefficient C(5,n) in seq_t. */
    seq_t c = 1;
    idx_t k = n;
    /* Use symmetry to reduce loop iterations. */
    if (k > 5 - k)
        k = 5 - k;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* Multiply first to preserve integer division. */
        c = (c * ((seq_t)5 - i + 1)) / i;
    }

    return c * E[5 - n];
}
