/*
 * A240685  Number of forests with n labeled nodes and 8 trees.
 * https://oeis.org/A240685
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n-16) • (n-7)(n-6)(n-5)(n-4)(n-3)(n-2)(n-1) • (n^7 +
 *            63n^6 + 2135n^5 + 49245n^4 + 816256n^3 + 9527868n^2 + 71254800n
 *            + 259459200) / 645120 (valid for n ≥ 8)
 * Range:     u8 n <= 9, u16 n <= 11, u32 n <= 14, u64 n <= 20, u128 n <= 30,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 8)
        return (seq_t)0;

    /* Polynomial coefficients for P(n) = n^7 + 63n^6 + … */
    const unsigned long long coeffs[7] = {
        63ULL, 2135ULL, 49245ULL, 816256ULL,
        9527868ULL, 71254800ULL, 259459200ULL
    };

    /* If seq_t is 64‑bit or smaller, use 128‑bit intermediate
       to avoid overflow for the values that fit into 64 bits. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 sn = (unsigned __int128)n;
        unsigned __int128 prod = 1;

        /* power part: n^(n-16) (possibly negative exponent) */
        unsigned __int128 pow_factor = 1;
        if (n >= 16) {
            for (idx_t i = 0; i < n - 16; ++i)
                pow_factor *= sn;
            prod *= pow_factor;
        } else {
            for (idx_t i = 0; i < 16 - n; ++i)
                pow_factor *= sn;
            /* division will be performed after other multiplications */
        }

        /* multiply (n-7)…(n-1) */
        for (idx_t i = 1; i <= 7; ++i)
            prod *= sn - (unsigned __int128)i;

        /* polynomial P(n) using Horner's rule */
        unsigned __int128 P = 1;
        for (int j = 0; j < 7; ++j)
            P = P * sn + coeffs[j];
        prod *= P;

        /* handle negative exponent (division) */
        if (n < 16)
            prod /= pow_factor;

        /* final division by 645120 */
        prod /= 645120ULL;

        return (seq_t)prod;
    }

    /* seq_t is an arbitrary‑precision integer.  All operations are
       performed using seq_t; big_pow handles large exponentiation. */
    seq_t sn = (seq_t)n;
    seq_t prod = (seq_t)1;

    /* power part: n^(n-16) (possibly negative exponent) */
    seq_t pow_factor;
    if (n >= 16)
        pow_factor = big_pow(sn, (seq_t)(n - 16));
    else
        pow_factor = big_pow(sn, (seq_t)(16 - n));

    if (n >= 16)
        prod *= pow_factor;   /* positive exponent */
    /* multiplication by product of (n-7)…(n-1) */
    for (idx_t i = 1; i <= 7; ++i)
        prod *= sn - (seq_t)i;

    /* polynomial P(n) using Horner's rule */
    seq_t P = (seq_t)1;
    for (int j = 0; j < 7; ++j)
        P = P * sn + (seq_t)coeffs[j];
    prod *= P;

    if (n < 16)
        prod /= pow_factor;   /* negative exponent */

    /* final division by 645120 */
    prod /= (seq_t)645120ULL;

    return prod;
}
