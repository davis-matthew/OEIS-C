/*
 * A125097  a(n) is k such that k*(2n+1) mod 1000 = 123, or 2n+1 if no such k
 *          exists.
 * https://oeis.org/A125097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 499, u32 n <= 499, u64 n <= 499,
 *            u128 n <= 499, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = k such that k*(2n+1) ≡ 123 (mod 1000);
   if no such k exists, a(n) = 2n+1 */

/* Computes the n-th term of OEIS sequence A125097. */
seq_t a(idx_t n)
{
    const idx_t MOD    = 1000;
    const idx_t TARGET = 123;

    /* (2n+1) mod 1000 depends only on n mod 500 */
    idx_t m_mod = ((n % 500) * 2 + 1) % MOD;

    /* Compute gcd(m_mod, MOD) */
    idx_t a0 = m_mod, b = MOD;
    while (b != 0) {
        idx_t t = a0 % b;
        a0 = b;
        b = t;
    }

    /* If gcd != 1, no solution; return 2n+1 */
    if (a0 != 1) {
        return ((seq_t)n * 2) + 1;
    }

    /* Find the modular inverse of m_mod modulo MOD (brute force). */
    idx_t inv = 0;
    for (idx_t x = 1; x < MOD; ++x) {
        if ((m_mod * x) % MOD == 1) {
            inv = x;
            break;
        }
    }

    /* k = inv * TARGET (mod MOD) */
    seq_t k = (seq_t)((inv * TARGET) % MOD);
    return k;
}
