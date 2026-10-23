/*
 * A177041  Sum(round(k^2/n),k=1..n).
 * https://oeis.org/A177041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum(round(k^2 / n), k = 1..n)
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* round(x) is defined as floor(x + 1/2) */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* undefined below offset */

    seq_t sum = 0;
    seq_t n_seq = (seq_t)n;            /* typed division */
    seq_t n_half = (seq_t)(n / 2); /* floor(n/2) works for rounding */

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t kk = (seq_t)k * k;        /* k^2 in seq_t */
        sum += (kk + n_half) / n_seq;  /* floor((k^2 + n/2)/n) */
    }
    return sum;
}
