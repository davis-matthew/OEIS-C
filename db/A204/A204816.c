/*
 * A204816  Final nonzero digit of n^n in base 6.
 * https://oeis.org/A204816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = final non‑zero digit of n^n in base 6 * For n>=1: let v2
 *            be the exponent of 2 in n, v3 the exponent of 3 in n, * and m =
 *            n / 2^v2 / 3^v3 (coprime to 6). * If v2 > v3: * let e =
 *            n*(v2-v3); 2^e mod 6 is 2 if e is odd else 4. * a(n) = (2^e mod
 *            6) * (m^n mod 6) mod 6. * m^n mod 6 is 1 if m≡1 (...
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)          /* handle 0^0 case */
        return (seq_t)1;

    idx_t temp = n;
    idx_t v2 = 0, v3 = 0;

    /* count factors of 2 */
    while ((temp & 1ULL) == 0)
    {
        v2++;
        temp >>= 1;
    }

    /* count factors of 3 */
    while (temp % 3ULL == 0)
    {
        v3++;
        temp /= 3;
    }

    /* temp now equals m (coprime to 6) */
    idx_t m_mod6 = temp % 6ULL;
    seq_t mpow = 1;
    if (m_mod6 == 5ULL)        /* m ≡ 5 (mod 6) */
        mpow = (n & 1ULL) ? (seq_t)5 : (seq_t)1;
    /* mpow = 1 if m mod 6 == 1, otherwise 5 if n odd else 1 */

    seq_t res;
    if (v2 > v3)   /* e > 0 */
    {
        /* parity of e = parity(n) * parity(v2-v3) */
        idx_t parity_delta = ((v2 - v3) & 1ULL);
        idx_t parity_e     = ((n & 1ULL) & parity_delta);
        seq_t two_pow = parity_e ? (seq_t)2 : (seq_t)4; /* 2^e mod 6 */
        res = (seq_t)((two_pow * mpow) % (seq_t)6);
    }
    else if (v2 < v3)   /* e > 0, 3^e mod 6 = 3 */
    {
        res = (seq_t)(( (seq_t)3 * mpow ) % (seq_t)6);
    }
    else                /* v2 == v3 */
    {
        res = mpow;
    }

    return res;
}
