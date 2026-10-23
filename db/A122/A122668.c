/*
 * A122668  a(n) = d(n)_n where d() = A000005.
 * https://oeis.org/A122668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = d(n)_n where d() = A000005 (number of divisors). That is,
 *            write d(n) in decimal digits and interpret that string as a
 *            base‑n number
 * Range:     u8 n <= 239, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>


seq_t a(idx_t n) {
    if (n == 0)
        return 0; /* safe guard – offset starts at 1 */

    /* ----- Compute the divisor count d(n) ----- */
    seq_t tau = 1;
    idx_t temp = n;
    idx_t p = 2;
    while (p <= temp / p) {
        if (temp % p == 0) {
            unsigned exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            tau *= (seq_t)(exp + 1);
        }
        p++;
    }
    if (temp > 1)
        tau *= (seq_t)2;   /* remaining prime factor */

    /* ----- Convert tau to its decimal digits ----- */
    unsigned char digits[128];  /* ample space for all practical cases */
    size_t num_digits = 0;
    seq_t t = tau;
    while (t > 0) {
        digits[num_digits++] = (unsigned char)(t % 10);
        t /= 10;
    }

    /* ----- Interpret digits as a base‑n number ----- */
    seq_t base = (seq_t)n;
    seq_t result = 0;
    for (size_t i = 0; i < num_digits; ++i) {
        result = result * base + (seq_t)digits[num_digits - 1 - i];
    }
    return result;
}
