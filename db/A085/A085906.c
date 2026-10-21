/*
 * A085906  Ramanujan sum c_n(6).
 * https://oeis.org/A085906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Ramanujan sum c_n(6). Computed multiplicatively: * a(1) =
 *            1 * For prime power p^k: * p == 2: a(2) = 1 * a(4) = -2 * a(2^k)
 *            = 0 for k >= 3 * p == 3: a(3) = 2 * a(9) = -3 * a(3^k) = 0 for k
 *            >= 3 * p >= 5: a(p) = -1 * a(p^k) = 0 for k >= 2 * The sequence
 *            is multiplicative: a(mn) = a(m)·a(n) f...
 * Range:     i8 n <= 9692, i16 n <= 9692, i32 n <= 9692, i64 n <= 9692,
 *            i128 n <= 9692, big any n
 * Helpers:   compute_prime_power_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/compute_prime_power_value.h"

seq_t a(idx_t n) {
    if (n == (idx_t)1) return (seq_t)1;

    seq_t result = (seq_t)1;
    uint64_t m = (uint64_t)n;

    /* factor 2 separately */
    if (m % 2 == 0) {
        idx_t cnt = 0;
        while (m % 2 == 0) {
            m /= 2;
            ++cnt;
        }
        seq_t val = compute_prime_power_value(2, cnt);
        if (val == (seq_t)0) return (seq_t)0;
        result *= val;
    }

    /* odd factors */
    uint64_t p = 3;
    while (p * p <= m) {
        if (m % p == 0) {
            idx_t cnt = 0;
            while (m % p == 0) {
                m /= p;
                ++cnt;
            }
            seq_t val = compute_prime_power_value((seq_t)p, cnt);
            if (val == (seq_t)0) return (seq_t)0;
            result *= val;
        }
        p += 2;
    }

    /* remaining prime > sqrt */
    if (m > 1) {
        seq_t val = compute_prime_power_value((seq_t)m, (idx_t)1);
        if (val == (seq_t)0) return (seq_t)0;
        result *= val;
    }

    return result;
}
