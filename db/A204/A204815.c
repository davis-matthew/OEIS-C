/*
 * A204815  Final nonzero digit of n^n in base 5.
 * https://oeis.org/A204815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = (n / 5^v5(n))^n mod 5, where v5(n) is the exponent of 5
 *            in n. Since 5^4 ≡ 1 (mod 5), the exponent n can be reduced
 *            modulo 4 (use 4 when n ≡ 0 (mod 4) and n > 0). Special case:
 *            a(0) = 1
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Remove all factors of 5 from n, leaving the coprime part m. */
    idx_t m = n;
    while (m % (idx_t)5 == 0)
        m /= (idx_t)5;

    /* m is now coprime to 5; only m mod 5 matters for the final result. */
    seq_t m_mod = (seq_t)(m % (idx_t)5);    /* m_mod ∈ {1,2,3,4} */

    /* Reduce the exponent n modulo 4, using 4 when n ≡ 0 (mod 4). */
    idx_t exp_mod = n % (idx_t)4;
    if (exp_mod == 0)
        exp_mod = 4;    /* n > 0 because n == 0 case was handled above */

    /* Compute m_mod^exp_mod mod 5.  Since exp_mod ≤ 4, a simple loop suffices. */
    seq_t res = (seq_t)1;
    while (exp_mod--)
        res = (res * m_mod) % (seq_t)5;

    return res;
}
