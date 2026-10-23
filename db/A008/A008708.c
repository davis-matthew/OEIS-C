/*
 * A008708  11 in base 11-n.
 * https://oeis.org/A008708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, nonn, base, full
 * Formula:   a(n) = decimal value of the representation of 11 in base (11 -
 *            n)
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 9, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* compute base = 11 - n as a signed 64‑bit value */
    long long base = (long long)11 - (long long)n;
    seq_t result = 0;

    if (base == 0) {                 /* undefined base : return 0 */
        return result;
    }

    /* unary representation of 11 (base == 1) */
    if (base == 1) {
        for (int i = 0; i < 11; ++i) {
            result = result * (seq_t)10 + (seq_t)1;
        }
        return result;
    }

    /* positive base (> 1) */
    if (base > 1) {
        long long x = 11;
        int digits[16];
        int cnt = 0;
        while (x > 0) {
            long long rem = x % base;
            digits[cnt++] = (int)rem;
            x /= base;
        }
        for (int i = cnt - 1; i >= 0; --i) {
            int d = digits[i];
            if (d < 10) {
                result = result * (seq_t)10 + (seq_t)d;
            } else {
                result = result * (seq_t)10 + (seq_t)(d / 10);
                result = result * (seq_t)10 + (seq_t)(d % 10);
            }
        }
        return result;
    }

    /* negative base (< -1) */
    long long x = 11;
    int digits[16];
    int cnt = 0;
    long long abs_base = -base;          /* base is negative */
    while (x != 0) {
        long long rem = x % abs_base;
        if (rem < 0) rem += abs_base;
        digits[cnt++] = (int)rem;
        x = (x - rem) / base;           /* base is negative */
    }
    for (int i = cnt - 1; i >= 0; --i) {
        int d = digits[i];
        if (d < 10) {
            result = result * (seq_t)10 + (seq_t)d;
        } else {
            result = result * (seq_t)10 + (seq_t)(d / 10);
            result = result * (seq_t)10 + (seq_t)(d % 10);
        }
    }
    return result;
}
