/*
 * A064435  Duplicate of A020488.
 * https://oeis.org/A064435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n-th number m with tau(m) = phi(m); finite sequence (7
 *            terms), found by direct search over m <= 100
 * Range:     i8 n <= 7, i16 n <= 7, i32 n <= 7, i64 n <= 7, i128 n <= 7,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t A020488_a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    idx_t count = 0;
    for (int m = 1; m <= 100; m++) {
        int t = m, phi = m, tau = 1;
        for (int p = 2; p * p <= t; p++) {
            if (t % p == 0) {
                int c = 0;
                while (t % p == 0) { t /= p; c++; }
                phi -= phi / p;
                tau *= (c + 1);
            }
        }
        if (t > 1) {
            phi -= phi / t;
            tau *= 2;
        }
        if (phi == tau) {
            count++;
            if (count == n)
                return (seq_t)m;
        }
    }
    return (seq_t)0; /* sequence is finite */
}

seq_t a(idx_t n)
{
    return A020488_a(n);
}
