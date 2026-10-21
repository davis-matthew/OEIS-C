/*
 * A226756  Number of elements X in the matrix ring M_2(Z_n) such that X^2 ==
 *          X (mod n).
 * https://oeis.org/A226756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} [ 2 + (p^2 + p) * p^(2e-2) ]
 * Range:     u8 n <= 9, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative: the number of idempotent 2×2 matrices over Z_n. */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t m = n;

    /* handle factor 2 separately for speed */
    if (m % 2 == 0) {
        idx_t e = 0;
        while (m % 2 == 0) {
            m /= 2;
            ++e;
        }
        seq_t p = 2;
        seq_t p2 = p * p;          /* p^2            */
        seq_t const_term = p2 + p; /* p^2 + p        */
        seq_t pow_p = 1;
        for (idx_t i = 0; i < 2 * e - 2; ++i) pow_p *= p; /* p^(2e-2) */
        seq_t term = 2 + const_term * pow_p;
        result *= term;
    }

    /* odd primes */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            seq_t P = p;
            seq_t P2 = P * P;
            seq_t const_term = P2 + P;
            seq_t pow_p = 1;
            for (idx_t i = 0; i < 2 * e - 2; ++i)
                pow_p *= P;            /* P^(2e-2) */
            seq_t term = 2 + const_term * pow_p;
            result *= term;
        }
    }

    /* remaining prime factor > sqrt(m) */
    if (m > 1) {
        seq_t P = m;
        seq_t P2 = P * P;
        seq_t term = 2 + (P2 + P);   /* e = 1, so p^(2e-2) = 1 */
        result *= term;
    }

    return result;
}
