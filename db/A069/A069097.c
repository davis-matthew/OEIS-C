/*
 * A069097  Moebius transform of A064987, n*sigma(n).
 * https://oeis.org/A069097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = Σ_{d|n} d² · φ(n/d) = Σ_{k=1..n} gcd(n,k)² Multiplicative
 *            with a(pᵉ) = p^(e‑1)·(pᵉ·(p+1) – 1) (OEIS A069097)
 * Range:     u8 n <= 13, u16 n <= 179, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;          /* a(1) = 1 */

    seq_t result = 1;
    idx_t nn = n;

    /* trial division to factor n */
    for (idx_t p = 2; p * p <= nn; ++p) {
        if (nn % p == 0) {
            idx_t e = 0;
            while (nn % p == 0) { ++e; nn /= p; }

            /* compute p^e and p^(e-1) */
            seq_t p_pow = 1;
            for (idx_t i = 0; i < e; ++i) p_pow *= (seq_t)p;
            seq_t p_pow_minus1 = p_pow / (seq_t)p;  /* p^(e-1) */

            /* term = p^(e-1) * (p^e·(p+1) – 1) */
            seq_t term = p_pow_minus1 * (p_pow * ((seq_t)(p + 1)) - (seq_t)1);
            result *= term;
        }
    }

    /* remaining prime factor (if any) */
    if (nn > 1) {
        seq_t p_pow = (seq_t)nn;                     /* p¹ */
        seq_t term = (p_pow * ((seq_t)(nn + 1)) - (seq_t)1); /* p^(0) =1 */
        result *= term;
    }

    return result;
}
