/*
 * A072877  a(1) = a(2) = a(3) = a(4) = 1; a(n) = (a(n-1)*a(n-3) +
 *          a(n-2)^4)/a(n-4).
 * https://oeis.org/A072877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (a(n-1)*a(n-3) + a(n-2)^4) / a(n-4), * with
 *            a(1)=a(2)=a(3)=a(4)=1. * This implementation evaluates the
 *            recurrence iteratively, keeping * only the four most recent
 *            terms. No dynamic allocation is required, * so it works for all
 *            supported seq_t types, including an * arbitrary‑precision
 *            integ...
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 10, u64 n <= 11, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)         /* undefined for n < 1, return 0 for safety */
        return (seq_t)0;

    /* Base values */
    if (n <= 4)
        return (seq_t)1;

    seq_t a1 = (seq_t)1;  /* a(n-4) */
    seq_t a2 = (seq_t)1;  /* a(n-3) */
    seq_t a3 = (seq_t)1;  /* a(n-2) */
    seq_t a4 = (seq_t)1;  /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i)
    {
        /* Compute a(n) = (a4*a2 + a3^4) / a1 */
        seq_t prod1 = a4 * a2;          /* a(n-1)*a(n-3) */

        seq_t sq = a3 * a3;             /* a(n-2)^2 */
        seq_t fourth = sq * sq;         /* a(n-2)^4 */

        seq_t numer = prod1 + fourth;   /* numerator */

        seq_t ai = numer / a1;          /* division is exact */

        /* Slide window: old a2 becomes a1, etc. */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = ai;
    }

    return a4;   /* a(n) */
}
