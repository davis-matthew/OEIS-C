/*
 * A118629  Number of n-digit integers with decimal digits in increasing
 *          order.
 * https://oeis.org/A118629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   a(n) = binomial(10, n) * (10 - n) / 10 (for n > 1) a(1) = 10.
 *            For n > 10 the value is 0
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)10;
    }
    if (n == 0 || n > 10) {
        return (seq_t)0;
    }

    /* Precomputed binomial coefficients choose(10, n) for n = 0..10 */
    static const seq_t choose[11] = {
        (seq_t)1,  (seq_t)10, (seq_t)45, (seq_t)120, (seq_t)210,
        (seq_t)252, (seq_t)210, (seq_t)120, (seq_t)45,  (seq_t)10,
        (seq_t)1
    };

    seq_t bin = choose[n];
    seq_t mult = (seq_t)(10 - n);           /* 10 - n is non‑negative for n ≤ 10 */
    return (bin * mult) / (seq_t)10;
}
