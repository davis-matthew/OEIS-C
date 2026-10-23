/*
 * A047194  Number of nonempty subsets of {1,2,...,n} in which exactly 1/3 of
 *          the elements are <= n/3.
 * https://oeis.org/A047194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{⌊n/3⌋} binomial(⌊n/3⌋, k) * binomial(n-⌊n/3⌋,
 *            2k)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts nonempty subsets of {1,…,n} with exactly one third of the elements ≤ n/3. */
seq_t a(idx_t n)
{
    idx_t m = n / 3;            /* ⌊n/3⌋ */
    idx_t N2 = n - m;           /* n - ⌊n/3⌋ */
    seq_t comb_m = 1;           /* C(m, 0) */
    seq_t comb_n = 1;           /* C(N2, 0) */
    seq_t sum = 0;

    for (idx_t k = 1; k <= m; ++k) {
        /* Update C(m, k) using multiplicative recurrence */
        comb_m = comb_m * (seq_t)(m - k + 1) / (seq_t)k;

        /* Update C(N2, 2k) from C(N2, 2k-2) in two steps */
        comb_n = comb_n * (seq_t)(N2 - (2 * k - 2)) / (seq_t)(2 * k - 1);
        comb_n = comb_n * (seq_t)(N2 - (2 * k - 1)) / (seq_t)(2 * k);

        /* Add term to the sum */
        sum += comb_m * comb_n;
    }

    return sum;
}
