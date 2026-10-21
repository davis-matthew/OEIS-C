/*
 * A215265  a(n) = (n-1)^(n+1) - n^n.
 * https://oeis.org/A215265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (n-1)^(n+1) - n^n
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 9, i64 n <= 15, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* fast exponentiation for seq_t; works for arbitrary‑precision seq_t or
   fixed‑size seq_t (overflows tolerated for values beyond the tested range) */
static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)-2;      /* (-1)^1 - 1 = -2 */
    seq_t term1 = pow_seq((seq_t)(n - 1), n + 1);  /* (n-1)^(n+1) */
    seq_t term2 = pow_seq((seq_t)n, n);            /* n^n         */
    return term1 - term2;
}
