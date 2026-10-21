/*
 * A227479  Number of pairs of 2 X 2 matrices over Z/nZ that commute.
 * https://oeis.org/A227479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e || n} a(p^e) where * a(p^e) = p^{5e-2} * (p^{e+2}
 *            + p^{e+1} + p^e - p - 1) (multiplicative) * (p^k denotes p
 *            raised to the integer k, all operations in seq_t)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* undefined for n==0, guard anyway */
    seq_t result = 1;
    idx_t tmp = n;

    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }

            /* pe = p^e */
            seq_t pe = 1;
            for (idx_t i = 0; i < e; ++i)
                pe *= (seq_t)p;

            /* p_pow_5e_minus2 = p^(5e-2)   (e >= 1 → exponent >= 3) */
            idx_t exp = 5 * e - 2;
            seq_t p_pow_5e_minus2 = 1;
            for (idx_t i = 0; i < exp; ++i)
                p_pow_5e_minus2 *= (seq_t)p;

            /* inner = p^{e+2} + p^{e+1} + p^e - p - 1
             * Compute with pe already available */
            seq_t inner = pe * p * p + pe * p + pe - ( (seq_t)p + 1 );

            seq_t term = p_pow_5e_minus2 * inner;
            result *= term;
        }
    }

    if (tmp > 1) {                 /* remaining prime factor (e = 1) */
        idx_t p = tmp;
        seq_t pe = (seq_t)p;       /* p^1 */
        seq_t p_pow_5e_minus2 = 1; /* p^3 for e==1 */
        for (idx_t i = 0; i < 3; ++i)
            p_pow_5e_minus2 *= (seq_t)p;

        seq_t inner = pe * p * p + pe * p + pe - ( (seq_t)p + 1 );
        seq_t term = p_pow_5e_minus2 * inner;
        result *= term;
    }

    return result;
}
