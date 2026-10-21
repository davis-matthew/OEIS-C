/*
 * A076755  Nearest integer to the kurtosis excess of the divisors of n.
 * https://oeis.org/A076755
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nearest integer to N4 / N2^2, where * N2 = τ·S2 – S1², *
 *            N4 = τ³·S4 – 4·τ²·S1·S3 + 6·τ·S1²·S2 – 3·S1⁴, * τ = number of
 *            divisors, * Sk = sum of the k‑th powers of the divisors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n < 2) return 0;          /* behaviour for n < 2 is irrelevant */

    /* Factor n: prime factors and exponents. */
    uint64_t pf[64];
    uint64_t exp[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pf, exp, &cnt);

    /* compute τ and Sk (k = 1..4) using unsigned __int128 for safety. */
    unsigned __int128 tau = 1;
    unsigned __int128 S[5] = {0, 0, 0, 0, 0};  /* S[0] unused */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p  = pf[i];
        uint64_t e  = exp[i];
        tau *= (unsigned __int128)(e + 1);

        for (unsigned k = 1; k <= 4; ++k) {
            /* compute pk = p^k */
            unsigned __int128 pk = 1;
            for (uint64_t t = 0; t < k; ++t)
                pk *= p;

            /* compute term = sum_{j=0}^{e} pk^j */
            unsigned __int128 term = 0;
            unsigned __int128 pw   = 1;
            for (uint64_t j = 0; j <= e; ++j) {
                term += pw;
                pw   *= pk;
            }

            if (S[k] == 0)      /* first prime factor */
                S[k] = term;
            else
                S[k] *= term;
        }
    }

    /* All S[k] are now ready for k = 1..4. */
    unsigned __int128 S1 = S[1];
    unsigned __int128 S2 = S[2];
    unsigned __int128 S3 = S[3];
    unsigned __int128 S4 = S[4];

    /* N2 = τ·S2 – S1² */
    unsigned __int128 N2 = tau * S2 - S1 * S1;

    /* N4 = τ³·S4 – 4·τ²·S1·S3 + 6·τ·S1²·S2 – 3·S1⁴ */
    unsigned __int128 N4 = tau * tau * tau * S4
                      - (unsigned __int128)4 * tau * tau * S1 * S3
                      + (unsigned __int128)6 * tau * S1 * S1 * S2
                      - (unsigned __int128)3 * S1 * S1 * S1 * S1;

    /* denominator = N2² */
    unsigned __int128 denom = N2 * N2;

    /* Compute nearest integer to N4 / denom. */
    unsigned __int128 q   = N4 / denom;
    unsigned __int128 r   = N4 % denom;
    if ((r << 1) >= denom)
        q += 1;

    return (seq_t)q;
}
