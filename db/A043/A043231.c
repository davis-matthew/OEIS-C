/*
 * A043231  Numbers k such that 1 and 8 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A043231: a(n) = the nth integer k such that the decimal
   representation of k contains the substring "18" or "81",
   but the representation of k-1 does not.  The function
   generates terms on demand and is independent of call order. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/get_digits.h"   /* declares get_digits(seq_t, uint8_t *, size_t *) */

static bool has18or81(seq_t x)
{
    /* Return true if x contains the consecutive digits 1-8 or 8-1. */
    static uint8_t buf[128];
    size_t len;
    get_digits(x, buf, &len);            /* most significant digit first */
    for (size_t i = 0; i + 1 < len; ++i) {
        if ( (buf[i] == 1 && buf[i+1] == 8) ||
             (buf[i] == 8 && buf[i+1] == 1) )
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)      /* offset starts at 1; behaviour for n<1 is irrelevant */
        return 0;

    seq_t count = 0;
    idx_t k = 1;      /* iterate over 1,2,3,... */

    while (1) {
        bool cond_k  = has18or81((seq_t)k);
        bool cond_k1 = (k > 0) ? has18or81((seq_t)(k-1)) : false;

        if (cond_k && !cond_k1) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
}
