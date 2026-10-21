/*
 * A078477  Number of rational knots with n crossings and unknotting number =
 *          1 (chiral pairs counted only once).
 * https://oeis.org/A078477
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   For odd n ≥ 5: let n = 2k+1, k ≥ 2 → a(n) = 2^(k‑1) – 1 For even
 *            n: n = 4k (k ≥ 2) → a(n) = 2^(2k‑1) – 2 n = 4k+1 (k ≥ 1) → a(n)
 *            = 2^(2k‑1) – 1 n = 4k+2 or 4k+3 (k ≥ 0) → a(n) = 2^(2k) – 1
 *            Special cases: a(3) = 1, a(4) = 1 The function below uses
 *            big_pow to compute 2^exponent exactly
 * Range:     u8 n <= 19, u16 n <= 35, u32 n <= 67, u64 n <= 131,
 *            u128 n <= 259, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* Handle undefined or offset terms */
    if (n < 3) return 0;
    if (n == 3) return 1;
    if (n == 4) return 1;

    /* Compute the modulus with 4 and integer division to obtain k */
    idx_t rem = n & 3;         /* n % 4 */
    idx_t k   = n / 4;         /* floor(n / 4) */

    seq_t val;
    switch (rem) {
        case 0: /* n = 4k,  k ≥ 2 */
            val = big_pow((seq_t)2, (seq_t)(2 * k - 1));
            return val - 2;
        case 1: /* n = 4k+1, k ≥ 1 */
            val = big_pow((seq_t)2, (seq_t)(2 * k - 1));
            return val - 1;
        case 2: /* n = 4k+2 */
        case 3: /* n = 4k+3 */
            val = big_pow((seq_t)2, (seq_t)(2 * k));
            return val - 1;
        default:
            return 0; /* unreachable */
    }
}
