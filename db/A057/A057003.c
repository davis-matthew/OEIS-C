/*
 * A057003  Write the natural numbers in groups: 1; 2,3; 4,5,6; 7,8,9,10; ...
 *          and multiply the members of each group.
 * https://oeis.org/A057003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n*(n+1)/2)!/((n-1)*n/2)! – product of n consecutive
 *            integers starting at T_{n-1}+1, where T_k = k(k+1)/2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out‑of‑range guard – never used in tests */
    /* triangular number T_n = n*(n+1)/2; start = T_{n-1}+1 = T_n - n + 1 */
    seq_t start = ((seq_t)n * (n + 1) / 2) - n + 1;
    seq_t result = 1;
    for (idx_t k = 0; k < n; ++k) {
        result *= start + (seq_t)k;
    }
    return result;
}
