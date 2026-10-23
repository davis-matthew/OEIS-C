/*
 * A140255  Inverse Mobius transform of A014963.
 * https://oeis.org/A140255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = τ(n) + Σ_{p^e‖n} e * (p – 1) where τ(n) is the number of
 *            divisors of n
 * Range:     u8 n <= 256, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, but safe guard */

    idx_t temp = n;
    seq_t tau = 1;                 /* product of (e_i + 1) */
    seq_t sum = 0;                 /* Σ e_i * (p_i – 1) */

    for (idx_t p = 2; p <= temp / p; ++p)
    {
        if (temp % p == 0)
        {
            idx_t e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }
            tau  = tau  * ((seq_t)(e + 1));           /* update τ(n) */
            sum  = sum  + (seq_t)e * ((seq_t)p - 1);  /* add extra for prime‑power divisors */
        }
    }

    if (temp > 1)   /* remaining prime factor (exponent = 1) */
    {
        tau  = tau  * 2;
        sum  = sum  + ((seq_t)temp - 1);
    }

    return tau + sum;
}
