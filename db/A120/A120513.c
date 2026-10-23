/*
 * A120513  a(n) = min{j : A120502(j) = n}.
 * https://oeis.org/A120513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + Σ_{k=1}^{n-1} v₂(k) + 3·(# of powers of 2 ≤ n-1),
 *            a(1)=1
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* v₂(k) is the exponent of 2 in the prime factorisation of k. */

seq_t a(idx_t n)
{
    /* Offset is 1; a(1) is the first term. */
    if (n == 1) return 1;

    /* m = n-1 */
    idx_t m = n - 1;

    /* sum_{k=1}^{n-1} v₂(k) = Σ floor((n-1)/2^i) for i>=1 */
    seq_t sum_v2 = 0;
    for (idx_t div = 2; div <= m; div <<= 1) {
        sum_v2 += (seq_t)(m / div);
    }

    /* number of powers of two <= n-1 = floor_log2(n-1)+1 */
    idx_t cnt = 0;
    idx_t tmp = m;
    while (tmp) {          /* shift right until zero – counts bits */
        cnt++;
        tmp >>= 1;
    }

    /* a(n) = n + sum_v2 + 3*cnt */
    return (seq_t)n + sum_v2 + 3 * (seq_t)cnt;
}
