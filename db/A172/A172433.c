/*
 * A172433  Let u(n) = Sum [n/[sqrt k]] and v(n) = Sum [n/(sqrt k)] where the
 *          summation index k ranges from 1 to infinity, although both sums
 *          are actually finite. Here [a] denotes the integer part of a. Then
 *          a(n) = u(n) - v(n).
 * https://oeis.org/A172433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m=1}^{n} (2m+1)⌊n/m⌋ – Σ_{g=1}^{n} g ( ⌊n²/g²⌋ –
 *            ⌊n²/(g+1)²⌋ ) which equals the difference of u(n) = Σ_{k}
 *            ⌊n/⌊√k⌋⌋ and v(n) = Σ_{k} ⌊n/√k⌋
 * Range:     u8 n <= 14, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* not used for n ≥ 1, but harmless */

    seq_t u = 0;
    seq_t v = 0;
    seq_t nsq = (seq_t)n * (seq_t)n;   /* n² for the v‑sum */

    /* compute u(n) = Σ_{m=1}^{n} (2m+1) ⌊n/m⌋ */
    for (idx_t m = 1; m <= n; ++m) {
        idx_t f = n / m;                    /* ⌊n/m⌋ as integer */
        seq_t coef = (seq_t)(2 * m + 1);    /* 2m+1 as seq_t */
        u += coef * (seq_t)f;
    }

    /* compute v(n) = Σ_{g=1}^{n} g ( ⌊n²/g²⌋ – ⌊n²/(g+1)²⌋ ) */
    for (idx_t g = 1; g <= n; ++g) {
        seq_t gsq   = (seq_t)g * (seq_t)g;          /* g²   */
        seq_t div1  = nsq / gsq;                    /* ⌊n²/g²⌋   */
        idx_t gp1   = g + 1;
        seq_t gsq2  = (seq_t)gp1 * (seq_t)gp1;      /* (g+1)² */
        seq_t div2  = nsq / gsq2;                    /* ⌊n²/(g+1)²⌋ */
        seq_t diff  = div1 - div2;                  /* difference */
        v += (seq_t)g * diff;                      /* add g·diff */
    }

    return u - v;
}
