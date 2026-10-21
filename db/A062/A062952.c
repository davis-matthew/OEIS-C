/*
 * A062952  Multiplicative with a(p^e) = (p^(2*e+2)-p^(e+1)-p^e+p)/(p^2-1).
 * https://oeis.org/A062952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 8170, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_ui, prime_power_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Sum_{d|n} phi(d)*sigma(d)      (A062952)
 * The sequence is multiplicative: a(p^e) = (p^(2e+2) – p^(e+1) – p^e + p)/(p^2 – 1).
 */

#include <stdint.h>   /* for uint64_t */
#include "../utils/pow_ui.h"
#include "../utils/prime_power_value.h"

/* --- main sequence function --- */
seq_t a(idx_t n)
{
    /* the OEIS offset is 1; a(1) = 1 */
    if (n == 0) return 0;  /* not used, but harmless */
    if (n == 1) return (seq_t)1;

    uint64_t m = (uint64_t)n;
    seq_t result = 1;

    /* factorization: trial division */
    int exp;
    /* handle factor 2 separately */
    exp = 0;
    while ((m & 1ULL) == 0) {
        ++exp;
        m >>= 1;
    }
    if (exp > 0) {
        result *= prime_power_value(2, exp);
    }

    /* odd factors */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            exp = 0;
            while (m % p == 0) {
                ++exp;
                m /= p;
            }
            result *= prime_power_value(p, exp);
        }
    }

    /* remaining prime factor (if any) */
    if (m > 1) {
        result *= prime_power_value(m, 1);
    }

    return result;
}
