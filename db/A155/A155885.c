/*
 * A155885  "Look and Say" applied to the whole sequence so far.
 * https://oeis.org/A155885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, fini
 * Formula:   a(n) = concatenation_{d=2}^{n+1} ((n+1) || d)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where "||" denotes decimal concatenation of (n+1) with digit d. */
#include "../utils/pow10.h"     /* pow10(idx_t e) */
#include "../utils/digit_len.h" /* digit_len(seq_t x) */

seq_t a(idx_t n)
{
    /* We assume n >= 1, as per OEIS offset. */
    seq_t w = (seq_t)(n + 1);                /* the count value (n+1) */
    unsigned L = digit_len(w) + 1;           /* length of each component in digits */
    seq_t p10 = pow10(L);                    /* 10^L */

    seq_t result = 0;
    for (idx_t d = 2; d <= n + 1; ++d) {
        seq_t component = (w * 10) + (seq_t)d;
        result = (result * p10) + component;
    }
    return result;
}
