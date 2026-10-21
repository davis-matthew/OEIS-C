/*
 * A090561  Least n-th power that begins with k^(n-1) for some k > 1.
 * https://oeis.org/A090561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 10^n for n >= 6; for n < 6, a(n) is the least n-th power
 *            that begins with k^(n-1) for some k > 1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   digit_count, pow10, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/pow_uint.h"
#include "../utils/digit_count.h"

seq_t a(idx_t n) {
    /* Handle the special case n = 1 */
    if (n == 1) {
        return (seq_t)1;
    }

    /* For n >= 6 the minimal n-th power is 10^n */
    if (n >= 6) {
        return pow10(n);
    }

    /* For n = 2 .. 5 brute‑force m and k */
    for (uint64_t m = 2; ; ++m) {
        seq_t m_pow = pow_uint(m, (idx_t)n);
        uint64_t m_val = (uint64_t)m_pow;          /* fits in 64 bits for n < 6 */
        int digits_v = digit_count(m_val);

        for (uint64_t k = 2; ; ++k) {
            seq_t k_pow = pow_uint(k, (idx_t)(n - 1));
            uint64_t k_val = (uint64_t)k_pow;
            int digits_w = digit_count(k_val);

            /* If the prefix would be longer than the value, stop this k loop */
            if (digits_w > digits_v) {
                break;
            }

            /* Check if m_pow starts with k_pow */
            int diff = digits_v - digits_w;
            seq_t mult = pow10(diff);
            if (m_pow / mult == k_pow) {
                return m_pow;
            }
        }
    }
}
