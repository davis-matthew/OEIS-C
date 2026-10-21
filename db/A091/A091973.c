/*
 * A091973  Poincaré series [or Poincare series] (or Molien series) for a
 *          certain three-dimensional group of order 1344.
 * https://oeis.org/A091973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Poincaré series of a 3‑D group of order 1344. a(n) is obtained
 *            from the rational generating function (1 + x^2 + 3x^3 + 2x^4 +
 *            4x^5 + 5x^6 + 4x^7 + 5x^8 + 4x^9 + 2x^10 + 3x^11 + x^12 + x^14)
 *            / ((1−x^4)(1−x^6)(1−x^7)). This yields a linear recurrence of
 *            order 17: a[n] = a[n-4] + a[n-6] + a[n-7] - a...
 * Range:     u8 n <= 47, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static const int Ncoef[15] =
{
    1,  /* x^0 */
    0,  /* x^1 */
    1,  /* x^2 */
    3,  /* x^3 */
    2,  /* x^4 */
    4,  /* x^5 */
    5,  /* x^6 */
    4,  /* x^7 */
    5,  /* x^8 */
    4,  /* x^9 */
    2,  /* x^10 */
    3,  /* x^11 */
    1,  /* x^12 */
    0,  /* x^13 */
    1   /* x^14 */
};

seq_t a(idx_t n)
{
    /* static array holding already computed terms */
    static seq_t *seq = NULL;
    static size_t allocated = 0;      /* number of allocated slots */

    /* enlarge array if needed */
    if ((size_t)n + 1 > allocated)
    {
        size_t new_cap = (size_t)n + 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!seq) abort();   /* out‑of‑memory abort */
        /* compute new terms */
        for (size_t i = allocated; i <= (size_t)n; ++i)
        {
            seq_t Ni = 0;
            if (i < 15)           /* numerator coefficient */
                Ni = (seq_t)Ncoef[i];
            else
                Ni = 0;

            /* compute a[i] using the recurrence */
            seq_t val = Ni;

            if (i >= 4)  val += seq[i - 4];
            if (i >= 6)  val += seq[i - 6];
            if (i >= 7)  val += seq[i - 7];
            if (i >= 10) val -= seq[i - 10];
            if (i >= 11) val -= seq[i - 11];
            if (i >= 13) val -= seq[i - 13];
            if (i >= 17) val += seq[i - 17];

            seq[i] = val;
        }
        allocated = new_cap;
    }

    return seq[(size_t)n];
}
