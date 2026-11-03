/*
 * A218277  Convolution of level 3 of the divisor function.
 * https://oeis.org/A218277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/72)*(31*sigma3(n) - sigma3(3*n) + 7*sigma(n) -
 *            sigma(3*n) - 30*n*sigma(n) + 6*n*sigma(3*n))
 * Range:     u8 n <= 17, u16 n <= 107, u32 n <= 4031, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9543 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>   /* for sqrt on double */

/* Compute σ_k(n) as unsigned __int128 for k = 1 or 3 */
static inline unsigned __int128 sigma_k_uint128(idx_t n, unsigned char k)
{
    unsigned __int128 sum = 0;
    /* simple integer square root */
    idx_t root = 0;
    while ((root + 1) * (root + 1) <= n) ++root;

    for (idx_t d = 1; d <= root; ++d) {
        if (n % d == 0) {
            idx_t q = n / d;

            unsigned __int128 dpow = 1;
            for (unsigned char i = 0; i < k; ++i) dpow *= d;
            sum += dpow;

            if (q != d) {
                unsigned __int128 qpow = 1;
                for (unsigned char i = 0; i < k; ++i) qpow *= q;
                sum += qpow;
            }
        }
    }
    return sum;
}

/* External linkage function implementing the sequence */
seq_t a(idx_t n)
{
    /* σ(·) and σ₃(·) needed for n and 3n */
    unsigned __int128 sig_n   = sigma_k_uint128(n, 1);
    unsigned __int128 sig3_n  = sigma_k_uint128(n, 3);
    unsigned __int128 sig_3n  = sigma_k_uint128(3 * n, 1);
    unsigned __int128 sig3_3n = sigma_k_uint128(3 * n, 3);

    unsigned __int128 n_val = n;

    unsigned __int128 term =
          (31 * sig3_n)
        - sig3_3n
        + (7 * sig_n)
        - sig_3n
        - (30 * n_val * sig_n)
        + (6 * n_val * sig_3n);

    term /= 72;

    return (seq_t)term;
}
