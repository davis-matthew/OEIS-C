/*
 * A116690  a(n) = C(n,8) + C(n,7) + C(n,6) + C(n,5) + C(n,4) + C(n,3) +
 *          C(n,2) + C(n,1).
 * https://oeis.org/A116690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=1}^{8} C(n,k)
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 62, u64 n <= 966,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sum of binomial coefficients C(n,1)…C(n,8) for n ≥ 0. */
seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic internally to avoid overflow during
       intermediate multiplications. The final result is cast to seq_t
       – any overflow beyond the capacity of seq_t is intentionally
       un‑checked, matching the problem spec. */
    __uint128_t sum = 0;
    __uint128_t bin  = 1;          /* bin = C(n,0) initially */
    for (idx_t k = 1; k <= 8 && k <= n; ++k) {
        bin = bin * (__uint128_t)(n - k + 1) / k;   /* bin = C(n,k)   */
        sum += bin;                                 /* accumulate      */
    }
    return (seq_t)sum;
}
