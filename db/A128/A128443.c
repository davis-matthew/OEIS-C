/*
 * A128443  a(n) = floor(n*3^(n/2)).
 * https://oeis.org/A128443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*3^(n/2))
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

/* Compute 3^exp as a seq_t, using exponentiation by squaring. */
static seq_t pow3(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 3;
    while (exp > 0) {
        if (exp & 1ULL) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* safety for indices < 1, not used */

    if (n & 1ULL) {                /* odd n: a(n) = floor(n*3^((n-1)/2)*sqrt(3)) */
        idx_t exp = (n - 1) >> 1;  /* (n-1)/2 */
        seq_t pow3e = pow3(exp);
        seq_t m     = (seq_t)n * pow3e;   /* m = n * 3^((n‑1)/2) */
        seq_t t = m * m;                 /* m^2 */
        t = t * 3;                       /* m^2*3 */
        return floor_sqrt(t);            /* floor(sqrt(m^2*3)) = floor(m*sqrt(3)) */
    } else {                       /* even n: a(n) = n*3^(n/2) */
        idx_t exp = n >> 1;          /* n/2 */
        seq_t pow3e = pow3(exp);
        return (seq_t)n * pow3e;
    }
}
