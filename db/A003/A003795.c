/*
 * A003795  Order of simple Chevalley group A_n (5).
 * https://oeis.org/A003795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^(n(n+1)/2) * Product_{i=2..n+1} (5^i - 1) / gcd(n+1,
 *            4), a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t pow_seq(seq_t b, idx_t e) {
    seq_t r = 1;
    while (e) {
        if (e & 1) r *= b;
        e >>= 1;
        if (e) b *= b;
    }
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;

    idx_t g = 1;
    if ((n + 1) % 2 == 0) g = 2;
    if ((n + 1) % 4 == 0) g = 4;

    idx_t e = n * (n + 1) / 2;
    seq_t result = pow_seq((seq_t)5, e);

    /* g divides 5^2 - 1 = 24, so divide it out of the first factor exactly */
    result *= (seq_t)(24 / g);

    seq_t p = 25;
    for (idx_t i = 3; i <= n + 1; i++) {
        p *= 5;
        result *= (p - 1);
    }
    return result;
}
