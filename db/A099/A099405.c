/*
 * A099405  Decimal part of 1/a(n) starts with Fibonacci(n) (leading zeros
 *          excluded).
 * https://oeis.org/A099405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = minimal integer a such that the first non‑zero decimal
 *            digit of 1/a equals F(n) (the n‑th Fibonacci number). The
 *            computation is based on the inequalities (F+1)*a > 10^{m+L-1} ≥
 *            F*a together with the condition 10^{m-1} < a ≤ 10^m, where L =
 *            number of decimal digits of F(n)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   digit_len, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is a direct implementation of the algorithm described in the analysis. */

#include <stdbool.h>
#include "../utils/digit_len.h"
#include "../utils/fib.h"

seq_t a(idx_t n) {
    /* Fibonacci value and its decimal length */
    seq_t F = fib(n);
    int L = digit_len(F);           /* 1 ≤ L ≤ ~2100 for n≤10000 */

    /* Compute 10^L once */
    seq_t pow10L = 1;
    for (int i = 0; i < L; ++i) pow10L *= (seq_t)10;

    /* Initial values for m = 1 */
    seq_t T = pow10L;                /* 10^{m+L-1} when m=1 */
    seq_t pow10m_minus1 = 1;         /* 10^{m-1} */
    seq_t pow10m = 10;               /* 10^m */

    for (idx_t m = 1; ; ++m) {
        /* Bounds derived from the inequalities */
        seq_t div_low = F + (seq_t)1;      /* F+1 */
        seq_t lower = T / div_low;        /* floor(T/(F+1)) */
        lower += (seq_t)1;                /* ceil: floor(T/(F+1)) + 1 */

        /* Enforce a > 10^{m-1} */
        if (lower <= pow10m_minus1) {
            lower = pow10m_minus1 + (seq_t)1;
        }

        seq_t div_up = F;
        seq_t upper = T / div_up;      /* floor(T/F) */
        if (upper > pow10m) {
            upper = pow10m;
        }

        if (lower <= upper) {
            return lower;              /* minimal a for this m */
        }

        /* Advance to next m */
        T *= (seq_t)10;
        pow10m_minus1 = pow10m;
        pow10m *= (seq_t)10;
    }
}
