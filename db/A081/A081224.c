/*
 * A081224  Smallest k such that floor(k*e^Pi) begins with n
 *          (e^Pi=23.14069264...).
 * https://oeis.org/A081224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = min{ k ≥ 1 | floor(k*e^π) begins with the decimal *
 *            representation of n } (e^π ≈ 23.140692632578... )
 * Range:     u8 n <= 60, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* k will be searched starting from 1.  The multiplication uses a
 * 128‑bit integer to avoid overflow:  k * NUM  <  2^128.  The value
 * floor(k*C) is then obtained by integer division by DEN.
 * C is approximated by NUM/DEN with 12 fractional decimal digits,
 * which is sufficiently accurate for n up to 10 000 (k ≤ ~5000). */

seq_t a(idx_t n)
{
    /* number of decimal digits of n */
    idx_t nd = 0;
    idx_t tmp = n;
    do { nd++; tmp /= 10; } while (tmp);

    /* powers of 10 up to 10^18 */
    static const unsigned long long POW10[19] = {
        1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL, 100000ULL,
        1000000ULL, 10000000ULL, 100000000ULL, 1000000000ULL,
        10000000000ULL, 100000000000ULL, 1000000000000ULL,
        10000000000000ULL, 100000000000000ULL, 1000000000000000ULL,
        10000000000000000ULL, 100000000000000000ULL,
        1000000000000000000ULL
    };

    /* e^π approximated as NUM/DEN  */
    const unsigned long long NUM = 23140692632579ULL;   /* ceil(e^π * 10^12) */
    const unsigned long long DEN = 1000000000000ULL;    /* 10^12          */

    idx_t k = 1;
    for (;; ++k) {
        /* floor(k * e^π) */
        unsigned long long val =
            (unsigned long long)(((__uint128_t)k * NUM) / DEN);

        /* number of digits of val */
        unsigned long long temp = val;
        unsigned long long D = 0;
        do { D++; temp /= 10; } while (temp);

        /* leading nd digits of val */
        unsigned long long first = val / POW10[D - nd];

        if (first == (unsigned long long)n)
            return (seq_t)k;
    }
}
