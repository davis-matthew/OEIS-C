/*
 * A163163  a(n) = sigma(n) + tau(n) - n.
 * https://oeis.org/A163163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(n) + tau(n) - n
 * Range:     u8 n <= 119, u16 n <= 9157, u32 n <= 9157, u64 n <= 9157,
 *            u128 n <= 9157, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute sigma(n) and tau(n) by prime factorisation */
    seq_t sigma = 1;
    seq_t tau   = 1;
    idx_t temp = n;

    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            unsigned int exp = 0;
            seq_t sum_p = 1;
            seq_t pow = 1;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
                pow = pow * (seq_t)p;
                sum_p = sum_p + pow;
            }
            sigma = sigma * sum_p;
            tau   = tau * (seq_t)(exp + 1);
        }
    }

    if (temp > 1) {          /* remaining prime factor */
        sigma = sigma * ((seq_t)1 + temp);
        tau   = tau * (seq_t)2;
    }

    return sigma + tau - (seq_t)n;
}
