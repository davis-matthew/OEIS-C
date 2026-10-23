/*
 * A136695  Final nonzero digit of n! in base 8.
 * https://oeis.org/A136695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = final non‑zero digit of n! in base 8.
 * Computation:
 *   Let v2(n!) be the exponent of 2 in n!.
 *   Write n! = 8^k · m where m is not divisible by 8 (k = ⌊v2/3⌋).
 *   Then a(n) = m (mod 8) = (odd_part(n!) · 2^(v2 mod 3)) mod 8.
 */
seq_t a(idx_t n)
{
    seq_t odd_fact = 1;

    /* product of all odd parts of 1…n  (mod 8) */
    for (idx_t i = 1; i <= n; ++i) {
        idx_t x = i;
        while ((x & 1) == 0)           /* strip factors of 2 */
            x >>= 1;
        odd_fact = (odd_fact * (seq_t)x) & 7;   /* modulo 8 */
    }

    /* exponent of 2 in n! */
    idx_t v2 = 0;
    for (idx_t power = 2; power <= n; power <<= 1) {
        v2 += n / power;
        if (power > n / 2) break;    /* avoid overflow for very large n */
    }

    /* multiply by 2^(v2 mod 3) */
    idx_t r = v2 % 3;
    seq_t m = odd_fact;
    if (r == 1)
        m = (m * 2) & 7;  /* 2^1 == 2 (mod 8) */
    else if (r == 2)
        m = (m * 4) & 7;  /* 2^2 == 4 (mod 8) */

    return m;
}
