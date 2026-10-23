/*
 * A170386  Number of reduced words of length n in Coxeter group on 41
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^43 = I.
 * https://oeis.org/A170386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   power40
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(n)=41·40^(n‑1) for 1≤n≤42.
   For n≥43: a(n)=780·a(n‑43) – 39·∑_{k=1}^{42}a(n‑k). */
#include <stddef.h>
#include <stdint.h>
#include "../utils/power40.h"

/* seq_t and idx_t are provided by the build (not defined here). */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    /* Handle the first 42 terms directly. */
    if (n <= 42) {
        seq_t p = power40((idx_t)(n - 1));
        return (seq_t)41 * p;
    }

    /* Use a circular buffer of size 43 to store the last 43 terms. */
    seq_t buf[43];
    buf[0] = (seq_t)1;                /* a(0) */
    buf[1] = (seq_t)41;               /* a(1) */
    for (idx_t i = 2; i <= 42; ++i) {
        seq_t p = power40((idx_t)(i - 1));
        buf[i] = (seq_t)41 * p;       /* a(i) */
    }

    seq_t value = 0;
    for (idx_t i = 43; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= 42; ++k)
            sum += buf[(i - k) % 43];
        value = (seq_t)780 * buf[(i - 43) % 43] - (seq_t)39 * sum;
        buf[i % 43] = value;
    }
    return buf[n % 43];
}
