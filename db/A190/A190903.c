/*
 * A190903  a(n) = Product_{k in M_n} k, M_n = {k | 1 <= k <= 3n and k mod 3 =
 *          n mod 3}.
 * https://oeis.org/A190903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of numbers k with 1 ≤ k ≤ 3n and k mod 3 = n mod
 *            3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t res = (seq_t)1;
    unsigned int r = (unsigned int)(n % 3);

    if (r == 0) {
        /* multiples of 3: 3,6,…,3n */
        for (idx_t i = 1; i <= n; ++i)
            res *= (seq_t)(3 * i);
    } else {
        /* r = 1 or 2: r, r+3, …, r+3(n-1) */
        for (idx_t i = 0; i < n; ++i)
            res *= (seq_t)(r + 3 * i);
    }

    return res;
}
