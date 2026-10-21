/*
 * A054009  n read modulo (number of proper divisors of n).
 * https://oeis.org/A054009
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n mod (τ(n) - 1) for n > 1, where τ(n) is the number of
 *            divisors of n
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A simple divisor–counting routine is used; no external helpers are required. */

#include <stdint.h>  /* for uint64_t */

seq_t a(idx_t n)
{
    /* Undefined for n < 2; return 0 as a safe fallback. */
    if (n < 2) return 0;

    uint64_t tau = 1;          /* τ(n) starts at 1 (empty product)   */
    uint64_t temp = n;         /* Work on a copy of n                 */

    /* Handle factor 2 separately for efficiency */
    if (temp % 2 == 0) {
        uint64_t exp = 0;
        while (temp % 2 == 0) {
            temp /= 2;
            ++exp;
        }
        tau *= (exp + 1);
    }

    /* Check odd factors from 3 upwards */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            tau *= (exp + 1);
        }
    }

    /* If a prime factor > sqrt(n) remains, it contributes exponent 1 */
    if (temp > 1) {
        tau *= 2;
    }

    uint64_t mod = tau - 1;         /* proper divisors count */
    seq_t nm = (seq_t)n;
    seq_t md = (seq_t)mod;
    return nm % md;
}
