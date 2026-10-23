/*
 * A151746  G.f.: (1-2*x-5*x^2+4*x^3)/((1-4*x)*(1-x)^2).
 * https://oeis.org/A151746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n = 0, * a(n) = (4^(n+1) + 6*n + 14) / 9 for n > 0
 *            * from OEIS A151746 using the given generating function
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Small helper: binary exponentiation for seq_t with 64‑bit exponent. */
static seq_t seq_pow_u64(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0)
        return 1;
    seq_t pow = seq_pow_u64((seq_t)4, n + 1);
    seq_t n_seq = (seq_t)n;
    seq_t numerator = pow + 6 * n_seq + 14;
    return numerator / 9;
}
