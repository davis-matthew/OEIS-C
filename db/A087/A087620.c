/*
 * A087620  #{0<=k<=n: k*n is divisible by 4}.
 * https://oeis.org/A087620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of k (0 <= k <= n) such that 4 divides k*n. For n
 *            = 0, the only term is k = 0, which is divisible by 4. For n > 0:
 *            Let v = exponent of 2 in the factorization of n (v = v₂(n)). If
 *            v >= 2, then 2² divides n, so every k satisfies 4 | k*n; a(n) =
 *            n + 1. If v = 1, we need an additional f...
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the special case n == 0 */
    if (n == 0)
        return (seq_t)1;

    /* Compute v₂(n) : number of trailing zeros in n's binary representation */
    idx_t m = n;
    int v = 0;
    while ((m & (idx_t)1) == 0) {
        v++;
        m >>= 1;
    }

    if (v >= 2) {
        /* n is divisible by 4, all k ∈ [0,n] work */
        return ((seq_t)n) + 1;
    } else if (v == 1) {
        /* n is ≡ 2 mod 4, only even k work */
        return ((seq_t)(n / 2)) + 1;
    } else {
        /* n is odd, only k divisible by 4 work */
        return ((seq_t)(n / 4)) + 1;
    }
}
