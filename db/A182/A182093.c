/*
 * A182093  Partial sums of A005590.
 * https://oeis.org/A182093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = partial sums of A005590
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Gf:  a(x) = (x/(1-x)) * prod_{k>=0} (1 + x^{2^k} - x^{2^{k+1}}) */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    idx_t max_deg = n;                     /* indices 0 .. n inclusive */
    seq_t *coeff = (seq_t *)calloc(max_deg + 1, sizeof(seq_t));
    if (!coeff)
        return (seq_t)0;                   /* unlikely */

    coeff[0] = (seq_t)1;                   /* initialise to 1 */

    for (idx_t k = 0; (idx_t)1 << k <= max_deg; ++k)
    {
        idx_t shift1 = (idx_t)1 << k;      /* 2^k   */
        idx_t shift2 = shift1 << 1;        /* 2^(k+1) */

        seq_t *tmp = (seq_t *)calloc(max_deg + 1, sizeof(seq_t));
        if (!tmp)
        {
            free(coeff);
            return (seq_t)0;
        }

        for (idx_t i = 0; i <= max_deg; ++i)
        {
            seq_t v = coeff[i];
            if (v == (seq_t)0)
                continue;

            /* term 1 */
            tmp[i] += v;

            /* term +x^(2^k) */
            if (shift1 <= max_deg && i <= max_deg - shift1)
                tmp[i + shift1] += v;

            /* term -x^(2^(k+1)) */
            if (shift2 <= max_deg && i <= max_deg - shift2)
                tmp[i + shift2] -= v;
        }

        free(coeff);
        coeff = tmp;
    }

    seq_t result = (seq_t)0;
    for (idx_t i = 0; i < n; ++i)
        result += coeff[i];

    free(coeff);
    return result;
}
