/*
 * A012885  Suffixes of 357686312646216567629137 (all primes).
 * https://oeis.org/A012885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   a(n) = 357686312646216567629137 mod 10^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 24,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* The constant 357686312646216567629137 as a string. */
    static const char *M_str = "357686312646216567629137";
    static const idx_t M_len = sizeof("357686312646216567629137") - 1; /* number of digits */

    /* If the requested suffix length is less than or equal to the length
       of M, return that substring; otherwise return M itself because
       M < 10^n for n > M_len. */
    if (n <= M_len)
    {
        /* The maximum length of M is 24, so a buffer of 25 bytes is enough
           to hold any suffix plus the terminating NUL. */
        char buf[25];
        size_t start = M_len - (size_t)n;          /* where the suffix starts in M_str */
        memcpy(buf, M_str + start, (size_t)n);     /* copy the last n digits */
        buf[n] = '\0';                            /* terminate the string */
        return seq_from_str(buf);                  /* convert to seq_t */
    }
    else
    {
        return seq_from_str(M_str);                /* the whole number */
    }
}
