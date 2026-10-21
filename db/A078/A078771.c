/*
 * A078771  a(n) = A008475(n) - A001414(n).
 * https://oeis.org/A078771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{p^e‖n} (p^e - p*e) (additive)
 * Range:     u8 n <= 342, u16 n <= 9220, u32 n <= 9220, u64 n <= 9220,
 *            u128 n <= 9220, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t m = n;

    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            /* compute p^e */
            seq_t power = 1;
            for (idx_t i = 0; i < e; ++i) {
                power *= (seq_t)p;
            }
            /* add term p^e - p*e */
            result += power - (seq_t)p * (seq_t)e;
        }
    }

    /* any remaining prime factor contributes p^1 - p*1 = 0, so nothing to add */

    return result;
}
