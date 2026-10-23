/*
 * A014349  Inverse of 340th cyclotomic polynomial.
 * https://oeis.org/A014349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the power‑series expansion of
 *            1/Φ₃₄₀(x)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   apply_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation avoids computing the cyclotomic polynomial directly.
   It is based on the factorisation
      Φ₃₄₀(x) = ∏_{d|340} (x^{340/d} – 1)^{μ(d)},
   where μ is the Möbius function.  Hence
      1/Φ₃₄₀(x) = ∏_{d|340, μ(d)=1} 1/(x^{340/d} – 1)
                 ∏_{d|340, μ(d)=-1} (x^{340/d} – 1).
   The eight non‑zero μ‑divisors give the factors
      (x¹³⁴⁰ – 1), (x³⁴ – 1), (x¹⁰ – 1), (x⁴ – 1) – all inverted,
      (x¹⁷⁰ – 1), (x⁶⁸ – 1), (x²⁰ – 1), (x² – 1) – normal.
   We multiply the corresponding series modulo x³⁴⁰, using only
   additions/subtractions, so all intermediate values fit easily into seq_t. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/apply_factor.h"

seq_t a(idx_t n)
{
    static seq_t period[340];
    static bool initialized = false;
    if (!initialized) {
        /* Temporary arrays for intermediate series. */
        seq_t cur[340] = {0};
        seq_t tmp[340] = {0};

        /* Start with the constant series 1. */
        cur[0] = (seq_t)1;

        /* Apply the eight required factors in any order. */
        /* (x^340 – 1) inverted */
        apply_factor(tmp, cur, 340, true);
        for (size_t i = 0; i < 340; ++i) cur[i] = tmp[i];

        /* (x^34  – 1) inverted */
        apply_factor(tmp, cur, 34, true);
        for (size_t i = 0; i < 340; ++i) cur[i] = tmp[i];

        /* (x^10  – 1) inverted */
        apply_factor(tmp, cur, 10, true);
        for (size_t i = 0; i < 340; ++i) cur[i] = tmp[i];

        /* (x^4   – 1) inverted */
        apply_factor(tmp, cur, 4, true);
        for (size_t i = 0; i < 340; ++i) cur[i] = tmp[i];

        /* (x^170 – 1) normal */
        apply_factor(tmp, cur, 170, false);
        for (size_t i = 0; i < 340; ++i) cur[i] = tmp[i];

        /* (x^68  – 1) normal */
        apply_factor(tmp, cur, 68, false);
        for (size_t i = 0; i < 340; ++i) cur[i] = tmp[i];

        /* (x^20  – 1) normal */
        apply_factor(tmp, cur, 20, false);
        for (size_t i = 0; i < 340; ++i) cur[i] = tmp[i];

        /* (x^2   – 1) normal */
        apply_factor(tmp, cur, 2, false);
        for (size_t i = 0; i < 340; ++i) cur[i] = tmp[i];

        /* Copy the resulting period into the static buffer. */
        for (size_t i = 0; i < 340; ++i) period[i] = cur[i];
        initialized = true;
    }
    return period[n % 340];
}
