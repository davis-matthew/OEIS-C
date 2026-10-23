/*
 * A170130  Number of reduced words of length n in Coxeter group on 25
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^38 = I.
 * https://oeis.org/A170130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: For n = 0: a(0) = 1
 * For 1 ≤ n ≤ 37: a(n) = 25 · 24ⁿ⁻¹
 * For n = 38: a(38) = 25 · 24³⁷ − 300
 * For n ≥ 39: a(n) = Σ_{i=1}^{37} a(n−i) + 12 · a(n−38)   (linear recurrence)
 */

#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are supplied by the build system */

seq_t a(idx_t n)
{
    /* 0 – first term */
    if (n == 0) return (seq_t)1;

    /* 1 – 37 : 25·24ⁿ⁻¹ */
    if (n <= 37) {
        seq_t p = 1;                 /* 24⁰ */
        for (idx_t i = 1; i < n; ++i) p *= (seq_t)24;   /* 24ⁿ⁻¹ */
        return p * (seq_t)25;
    }

    /* 38 : 25·24³⁷ − 300 */
    if (n == 38) {
        seq_t p = 1;                 /* 24⁰ */
        for (idx_t i = 1; i <= 37; ++i) p *= (seq_t)24; /* 24³⁷ */
        return p * (seq_t)25 - (seq_t)300;
    }

    /* n ≥ 39 :  recurrence a(n) = Σ_{i=1}^{37} a(n−i) + 12·a(n−38)  */
    seq_t buffer[38];      /* buffer[0] = a(n‑38), … , buffer[37] = a(n‑1) */

    /* build initial values a(1)…a(38) */
    seq_t p = 1;                      /* 24⁰ */
    for (idx_t i = 1; i <= 37; ++i) {
        buffer[i - 1] = p * (seq_t)25;     /* a(i) = 25·24ⁱ⁻¹ */
        p *= (seq_t)24;                   /* next power of 24 */
    }
    buffer[37] = p * (seq_t)25 - (seq_t)300;   /* a(38) */

    /* iterate recurrence */
    for (idx_t cur = 39; cur <= n; ++cur) {
        seq_t sum = 0;
        for (idx_t i = 1; i < 38; ++i) sum += buffer[i];
        seq_t newval = sum + (seq_t)12 * buffer[0];

        /* shift buffer left and append newval */
        for (idx_t i = 0; i < 37; ++i) buffer[i] = buffer[i + 1];
        buffer[37] = newval;
    }

    return buffer[37];   /* a(n) */
}
