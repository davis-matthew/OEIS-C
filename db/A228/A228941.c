/*
 * A228941  The n-th convergent of CF(e) is the a(n)-th convergent of DCF(e),
 *          the delayed continued fraction of e.
 * https://oeis.org/A228941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cofr, more
 * Formula:   let b[0] = 2, b[k] = 1 if k % 3 ≠ 2 b[k] = 2 * ((k + 1) / 3) if
 *            k % 3 = 2 (k ≥ 1) a(1) = 1 for n ≥ 2: let ins = 0 if b[n‑1] == 1
 *            then ins = b[n] – 1 a(n) = a(n‑1) + 1 + ins This reproduces the
 *            OEIS sequence A228941
 * Range:     u8 n <= 28, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   cf_coeff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/cf_coeff.h"

seq_t a(idx_t n)
{
    if (n <= 1U) return (seq_t)1;

    seq_t res = (seq_t)1;            /* a(1) = 1 */

    for (idx_t k = 1; k < n; ++k)
    {
        seq_t coeff = cf_coeff(k);   /* b[k] */
        seq_t inc   = (seq_t)1;      /* the "+1" in the recurrence */

        if (coeff == (seq_t)1)
        {
            seq_t next = cf_coeff(k + 1); /* b[k+1] */
            inc += next - (seq_t)1;       /* inserted terms */
        }

        res += inc;                    /* update a(k+1) */
    }

    return res;
}
