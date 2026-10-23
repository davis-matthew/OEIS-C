/*
 * A042630  Numerators of continued fraction convergents to sqrt(845).
 * https://oeis.org/A042630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0) = 29 * a(1) = 14*a(0) + 1 = 407 * a(n) = c[(n‑1) mod L] *
 *            a(n‑1) + a(n‑2) for n ≥ 2 * where c[0],c[1],… are the
 *            continued‑fraction coefficients of sqrt(845) * after the initial
 *            term a0 = 29, and L is their period length
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Compute the continued‑fraction period of sqrt(845) once */
    static uint64_t period[200];
    static size_t period_len = 0;
    if (period_len == 0) {
        const int64_t D  = 845;
        const int64_t a0 = 29;
        int64_t m = 0, d = 1, a = a0;
        size_t i = 0;
        int64_t a_next;
        do {
            m = d * a - m;
            d = (D - m * m) / d;
            a_next = (a0 + m) / d;
            period[i++] = (uint64_t)a_next;
            a = a_next;
        } while (a_next != 2 * a0 && i < 200);
        period_len = i;
    }

    /* Handle the first two terms explicitly */
    if (n == 0) {
        return (seq_t)29;
    }
    seq_t a0 = (seq_t)29;

    uint64_t coeff0_val = period[0];
    seq_t coeff0 = (seq_t)coeff0_val;
    seq_t prev2 = a0;                                       /* a(0) */
    seq_t prev1 = coeff0 * prev2 + (seq_t)1;                 /* a(1) */

    if (n == 1) {
        return prev1;
    }

    /* Compute subsequent terms using the recurrence */
    for (idx_t i = 2; i <= n; ++i) {
        uint64_t coeff_val = period[(size_t)((i - 1) % period_len)];
        seq_t coeff = (seq_t)coeff_val;
        seq_t cur = coeff * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
