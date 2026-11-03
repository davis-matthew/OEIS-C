/*
 * A011585  Legendre symbol (n,19).
 * https://oeis.org/A011585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = Legendre symbol (n,19)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Validated: standard, pass, 97 terms through n = 972 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute (a|19) for a coprime to 19.
   Result is 1 or -1, as a seq_t. */
static seq_t legendre_mod19_unit(uint64_t a) {
    uint64_t a_mod = a % 19U;
    if (a_mod == 0) {
        return (seq_t)0;  /* should not happen for coprime a */
    }
    /* Euler's criterion: a^((19-1)/2) mod 19 */
    uint64_t base = a_mod;
    uint64_t exp  = 9U;           /* (19-1)/2 */
    uint64_t res  = 1U;
    while (exp) {
        if (exp & 1U) {
            res = (res * base) % 19U;
        }
        base = (base * base) % 19U;
        exp >>= 1U;
    }
    return (seq_t)((res == 1U) ? 1 : -1);
}

seq_t a(idx_t n) {
    /* Handle the offset (a(0)=0) and the zero at multiples of 19. */
    if (n == 0U) return (seq_t)0;
    if (n % 19U == 0U) return (seq_t)0;

    seq_t result = (seq_t)1;
    uint64_t tmp   = (uint64_t)n;

    /* Factor via trial division. */
    for (uint64_t p = 2U; p <= tmp / p; ++p) {
        if (tmp % p == 0U) {
            unsigned cnt = 0U;
            while (tmp % p == 0U) {
                ++cnt;
                tmp /= p;
            }
            if (cnt & 1U) {               /* odd exponent contributes */
                seq_t leg = legendre_mod19_unit(p);
                if (leg == (seq_t)0) return (seq_t)0;
                result *= leg;
            }
        }
    }
    /* Remaining large prime factor (if any). */
    if (tmp > 1U) {
        seq_t leg = legendre_mod19_unit(tmp);
        if (leg == (seq_t)0) return (seq_t)0;
        result *= leg;
    }

    return result;
}
