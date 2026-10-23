/*
 * A190901  a(n) = Product_{k in M_n} k; M_n = {k | 1 <= k <= 2n and k mod 2 =
 *          n mod 2}.
 * https://oeis.org/A190901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \prod_{k=1}^{n} (n even ? 2*k : 2*k-1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Product of all integers 1 ≤ k ≤ 2n having the same parity as n */
seq_t a(idx_t n)
{
    seq_t result = 1;
    if ((n & 1) == 0) {            /* n even: multiply even numbers 2,4,...,2n */
        for (idx_t k = 1; k <= n; ++k)
            result *= (seq_t)(2 * k);
    } else {                        /* n odd: multiply odd numbers 1,3,...,2n-1 */
        for (idx_t k = 1; k <= n; ++k)
            result *= (seq_t)(2 * k - 1);
    }
    return result;
}
