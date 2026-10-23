/*
 * A040484  Continued fraction for sqrt(507).
 * https://oeis.org/A040484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(0) = 22; for n > 0 the continued‑fraction coefficients for
 *            sqrt(507) repeat with period 6: 1, 1, 14, 1, 1, 44. a(n) = 22 if
 *            n == 0 a(n) = period[(n-1) mod 6] otherwise All operations use
 *            only integer arithmetic
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    static const seq_t period[6] = {
        (seq_t)1, (seq_t)1, (seq_t)14,
        (seq_t)1, (seq_t)1, (seq_t)44
    };

    if (n == 0)
        return (seq_t)22;

    return period[(size_t)((n - 1) % 6)];
}
