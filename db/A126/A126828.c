/*
 * A126828  Ramanujan numbers (A000594) read mod 81.
 * https://oeis.org/A126828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n^2 + k) * sigma_7(n)) mod 81 * where k = 9 if n % 3 ==
 *            2, otherwise k = 0. * sigma_7(n) is the sum of the 7th powers of
 *            the divisors of n
 * Range:     u8 n <= 560, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 94 terms through n = 9521 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sigma_7(n) modulo 81
 * Uses multiplicativity: sigma_7(p^e) = 1 + p^7 + p^14 + ... + p^(7e) (mod 81). */
static seq_t sigma7_mod81(idx_t n) {
    idx_t remaining = n;
    seq_t result = 1; /* multiplicative identity modulo 81 */

    for (idx_t p = 2; p * p <= remaining; ++p) {
        if (remaining % p == 0) {
            /* count exponent e of prime p */
            idx_t e = 0;
            while (remaining % p == 0) {
                remaining /= p;
                ++e;
            }
            /* compute p7 = p^7 (mod 81) */
            seq_t p_mod = (seq_t)(p % 81);
            seq_t p7 = 1;
            for (int i = 0; i < 7; ++i) {
                p7 = (p7 * p_mod) % 81;
            }
            /* compute sum of geometric series 1 + p7 + p7^2 + ... + p7^e */
            seq_t pow = 1;
            seq_t sum = 1; /* start with p7^0 */
            for (idx_t i = 0; i < e; ++i) {
                pow = (pow * p7) % 81;
                sum += pow;
                if (sum >= 81) sum -= 81;
            }
            result = (result * sum) % 81;
        }
    }
    /* if remaining > 1 it is a prime > sqrt(original n) with exponent 1 */
    if (remaining > 1) {
        seq_t p_mod = (seq_t)(remaining % 81);
        seq_t p7 = 1;
        for (int i = 0; i < 7; ++i) {
            p7 = (p7 * p_mod) % 81;
        }
        seq_t sum = 1 + p7;
        if (sum >= 81) sum -= 81;
        result = (result * sum) % 81;
    }

    return result;
}
#ifndef INCLUDED_SEQUENCES_A126828
#define INCLUDED_SEQUENCES_A126828

seq_t a(idx_t n) {
    /* n is guaranteed >= 1 */
    seq_t n_seq = (seq_t)n;
    seq_t n2_mod81 = (n_seq * n_seq) % 81;
    seq_t k = (n % 3 == 2) ? 9 : 0;
    seq_t sigma = sigma7_mod81(n);
    seq_t prod = (n2_mod81 + k) % 81;
    prod = (prod * sigma) % 81;
    return prod;
}

#endif /* INCLUDED_SEQUENCES_A126828 */
