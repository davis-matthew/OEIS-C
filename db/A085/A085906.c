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
 * Range:     i8 n <= 65537, i16 n <= 65537, i32 n <= 65537, i64 n <= 65537,
 *            i128 n <= 65537, big any n
 * Validated: standard, pass, 116 terms through n = 9692 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t compute_prime_power_value(seq_t p, idx_t exp) {
    if (p == (seq_t)2) {
        if (exp == 1) return (seq_t)1;
        if (exp == 2) return (seq_t)-2;
        return (seq_t)0;
    }
    if (p == (seq_t)3) {
        if (exp == 1) return (seq_t)2;
        if (exp == 2) return (seq_t)-3;
        return (seq_t)0;
    }
    /* p >= 5 */
    if (exp == 1) return (seq_t)-1;
    return (seq_t)0;
}

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
