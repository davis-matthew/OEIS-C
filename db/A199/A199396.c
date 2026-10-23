/*
 * A199396  Binary XOR of (3^k - 1)/2 as k varies from 1 to n.
 * https://oeis.org/A199396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binary XOR of (3^k - 1)/2 for 1 ≤ k ≤ n
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 the sequence is undefined; return 0 safely. */
    if (n == 0) return 0;

    seq_t term = (seq_t)1;   /* term = (3^k - 1)/2, starting with k = 1 */
    seq_t xor_sum = (seq_t)0;

    for (idx_t i = 1; i <= n; ++i) {
        xor_sum ^= term;                /* accumulate XOR */
        term = term * (seq_t)3 + (seq_t)1; /* t_{k+1} = 3*t_k + 1 */
    }
    return xor_sum;
}
