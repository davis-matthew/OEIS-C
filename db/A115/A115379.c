/*
 * A115379  Number of positive integers k < n such that n XOR k < n and
 *          gcd(n,k) is odd.
 * https://oeis.org/A115379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 384, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A115379: count of k (1 ≤ k ≤ n) such that (n XOR k) < n
 * and gcd(n, k) is odd.
 */
seq_t a(idx_t n)
{
    seq_t count = 0;
    /* parity of n: 1 if odd, 0 if even */
    const idx_t n_parity = n & 1U;

    for (idx_t k = 1; k <= n; ++k) {
        /* skip pairs where both n and k are even (gcd would be even) */
        if (n_parity == 0 && (k & 1U) == 0)
            continue;

        if ((n ^ k) < n)
            ++count;
    }

    return count;
}
