/*
 * A146088  Numbers k with the property that shifting the rightmost digit of k
 *          to the left end doubles the number.
 * https://oeis.org/A146088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0 For n > 0: j = floor((n-1)/8) + 1 m = (n-1) % 8 + 2
 *            a(n) = m * (10^{18*j} - 1) / 19 (This follows from N = 10n+m,
 *            19n = m(10^d-2) with d = 18j-1.)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 8, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 10 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* trivial term */

    static seq_t pow10_18;
    static int initialized = 0;
    if (!initialized) {
        pow10_18 = pow10(18);
        initialized = 1;
    }

    /* Compute block index j (1‑based) and the trailing digit m (2‑9). */
    idx_t j = (n - 1) / 8 + 1;
    idx_t m = (n - 1) % 8 + 2;

    /* Compute 10^(18*j) using exponentiation by squaring with the fixed base 10^18. */
    seq_t pow10_18j = ipow(pow10_18, (seq_t)j);

    /* Compute (10^(18*j) - 1) / 19 and multiply by the trailing digit m. */
    seq_t tmp = (pow10_18j - 1) / 19;
    return tmp * (seq_t)m;
}
