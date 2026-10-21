/*
 * A077025  a(n) = Sum_{k=1..n} floor(n/(k + 1/2)).
 * https://oeis.org/A077025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} ⌊ n / (k + ½) ⌋ Equivalent to a(n) =
 *            Σ_{k=1..n} ⌊ (2*n) / (2*k + 1) ⌋
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute the n-th term of sequence A077025
   n is unsigned 64‑bit, seq_t is the sequence value type */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for offset 1 but harmless */

    seq_t total = 0;             /* accumulator in seq_t */
    seq_t two_n = (seq_t)n * 2;   /* 2*n fits in seq_t for all usable n */

    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t denom = (2 * k) + 1;   /* odd denominator 2k+1 */
        seq_t q = two_n / denom;     /* floor division in seq_t */
        total += q;
    }

    return total;
}
