/*
 * A204315  Numbers j such that floor(j^(1/4)) divides j.
 * https://oeis.org/A204315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Let f(x) = (4*x^3 + 15*x^2 + 26*x)/3 (integer). For a given n,
 *            let k be the smallest x with f(x) ≥ n. Then a(n) = (k+1)^4 - 1 -
 *            k * (f(k) - n)
 * Range:     u8 n <= 48, u16 n <= 5755, u32 n <= 9602, u64 n <= 9602,
 *            u128 n <= 9602, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t f_val(idx_t x)
{
    seq_t t = (seq_t)4 * x * x * x;   /* 4*x^3   */
    t += (seq_t)15 * x * x;          /* +15*x^2 */
    t += (seq_t)26 * x;              /* +26*x   */
    return t / 3;                    /* divide by 3 */
}

seq_t a(idx_t n)
{
    idx_t k = 0;
    while (f_val(k) < n) ++k;

    seq_t pow = (seq_t)(k + 1);
    pow = pow * pow;   /* (k+1)^2   */
    pow = pow * pow;   /* (k+1)^4   */

    seq_t f = f_val(k);
    seq_t term = pow - 1 - (seq_t)k * (f - (seq_t)n);
    return term;
}
