/*
 * A235335  Evolution of 13094 using Eric Angelini's
 *          add-or-subtract-and-iterate procedure.
 * https://oeis.org/A235335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 28, u64 n <= 144, u128 n <= 667,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements OEIS A235335.
   a(1) = 13094.  For n > 1 the term is obtained by iterating the rule
   p := p + q if q > p, and p := p – q if q < p,
   where q is the decimal number formed by concatenating the absolute
   differences of adjacent digits of p (cyclic).  The function computes
   a(n) from scratch, using a static digit buffer that is reused
   between calls. */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static uint8_t *digit_buf = NULL;
static size_t   digit_cap = 0;

seq_t a(idx_t n)
{
    /* a(1) = 13094 */
    if (n == 1)
        return (seq_t)13094;

    seq_t p = (seq_t)13094;

    for (idx_t cur = 2; cur <= n; ++cur)
    {
        /* length of decimal representation of p */
        seq_t tmp = p;
        size_t len = 0;
        do
        {
            tmp /= (seq_t)10;
            ++len;
        } while (tmp != (seq_t)0);

        /* ensure buffer is large enough */
        if (digit_cap < len)
        {
            uint8_t *new_buf = (uint8_t *)realloc(digit_buf, len);
            if (!new_buf && len > 0)
                abort();          /* allocation failure */
            digit_buf = new_buf;
            digit_cap = len;
        }

        /* extract digits of p into digit_buf (least‑significant first) */
        tmp = p;
        for (size_t i = 0; i < len; ++i)
        {
            digit_buf[i] = (uint8_t)(tmp % (seq_t)10);
            tmp /= (seq_t)10;
        }

        /* build q by concatenating absolute differences */
        seq_t q = 0;
        if (len > 0)
        {
            for (size_t i = len; i > 1; --i)
            {
                unsigned diff = (digit_buf[i-1] > digit_buf[i-2]
                                 ? digit_buf[i-1] - digit_buf[i-2]
                                 : digit_buf[i-2] - digit_buf[i-1]);
                q = q * (seq_t)10 + (seq_t)diff;
            }
            unsigned diff = (digit_buf[0] > digit_buf[len-1]
                             ? digit_buf[0] - digit_buf[len-1]
                             : digit_buf[len-1] - digit_buf[0]);
            q = q * (seq_t)10 + (seq_t)diff;
        }

        /* update p according to rule */
        if (q > p)
            p = p + q;
        else if (q < p)
            p = p - q;
        /* if q == p, p stays unchanged */
    }

    return p;
}
