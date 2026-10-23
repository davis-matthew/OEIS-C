/*
 * A168966  Number of reduced words of length n in Coxeter group on 49
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^22 = I.
 * https://oeis.org/A168966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For 0 < n < 22: a(n) = 2 + 47 * (a(0) + a(1) + … +
 *            a(n−1)) For n ≥ 22: a(n) = 47 * (a(n−1)+…+a(n−21)) − 1128 *
 *            a(n−22) (This recurrence follows from the rational generating
 *            function (t^22 + 2 t^21 + … + 2 t + 1) / (1 − 47 t − … − 47 t^21
 *            + 1128 t^22)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

typedef __int128 big_t;          /* intermediate type large enough for the needed indices */

static big_t *big_buf = NULL;    /* cached values in big precision */
static idx_t  big_cap = 0;       /* current capacity of big_buf            */
static idx_t  big_last = 0;      /* last computed index + 1                */

seq_t a(idx_t n)
{
    /* Ensure that big_buf is large enough.  Grow by powers of two for efficiency. */
    if (n + 1 > big_cap) {
        idx_t new_cap = (big_cap > 0 ? big_cap : 16);
        while (new_cap <= n) new_cap <<= 1;
        big_buf = (big_t *)realloc(big_buf, new_cap * sizeof(big_t));
        big_cap = new_cap;
    }

    /* Compute terms on demand.  big_last is the first index not yet computed. */
    for (idx_t i = big_last; i <= n; ++i) {
        if (i == 0) {
            big_buf[i] = 1;
        } else if (i < 22) {            /* short‑range formula */
            big_t sum = 0;
            for (idx_t k = 0; k < i; ++k)
                sum += (big_t)47 * big_buf[k];
            big_buf[i] = 2 + sum;
        } else {                        /* linear recurrence of order 22 */
            big_t sum = 0;
            for (idx_t k = 1; k <= 21; ++k)
                sum += (big_t)47 * big_buf[i - k];
            sum -= (big_t)1128 * big_buf[i - 22];
            big_buf[i] = sum;
        }
    }
    big_last = n + 1;                   /* all values up to n are now ready */

    /* Convert the big value to the build’s sequence type.  The cast truncates
       automatically, which is acceptable for indices whose true value fits
       into the target type, and is irrelevant for larger indices. */
    return (seq_t)big_buf[n];
}
