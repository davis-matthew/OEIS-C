/*
 * A136698  Final nonzero digit of n! in base 12.
 * https://oeis.org/A136698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = (n! / 12^k) mod 12, where k = min(v2(n!)/2, v3(n!))
 * Range:     u8 n <= 9539, u16 n <= 9539, u32 n <= 9539, u64 n <= 9539,
 *            u128 n <= 9539, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Final non‑zero digit of n! in base 12. */

static seq_t mod_pow(seq_t base, idx_t exp, seq_t mod)
{
    seq_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1U)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Count powers of 2 and 3 in n! */
    idx_t v2 = 0, v3 = 0;
    for (idx_t p = 2; p <= n; p <<= 1)
        v2 += n / p;
    for (idx_t p = 3; p <= n; p *= 3)
        v3 += n / p;

    /* Trailing zeros in base 12 */
    idx_t k = (v2 / 2 < v3) ? v2 / 2 : v3;
    idx_t rem2 = v2 - 2 * k;
    idx_t rem3 = v3 - k;

    /* Product of numbers with all 2’s and 3’s stripped, modulo 12 */
    seq_t prod = 1;
    for (idx_t i = 2; i <= n; ++i) {
        idx_t j = i;
        while ((j & 1ULL) == 0)
            j >>= 1;
        while (j % 3ULL == 0)
            j /= 3;
        prod = (prod * (seq_t)(j % 12)) % 12;
    }

    /* Remaining powers of 2 and 3 modulo 12 */
    seq_t pow2 = mod_pow(2, rem2, 12);
    seq_t pow3 = mod_pow(3, rem3, 12);

    seq_t result = prod;
    result = (result * pow2) % 12;
    result = (result * pow3) % 12;
    return result;
}
