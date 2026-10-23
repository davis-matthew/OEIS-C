/*
 * A044199  Numbers n such that string 1,4 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose base‑8 representation *
 *            contains the substring "14" while the base‑8 representation of
 *            the * predecessor does not. The function finds this term by
 *            scanning * successive integers until it has found n such terms
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Return true iff x (in base 8) contains the adjacent digits 1 followed
 * by 4.  We scan from least significant to most significant digit; the
 * pattern 1,4 in normal order corresponds to seeing a 4 then a 1 while
 * traversing. */
static bool has_14(seq_t x)
{
    seq_t prev = (seq_t)8;          /* sentinel value outside [0,7] */
    while (x > (seq_t)0) {
        seq_t d = x % (seq_t)8;
        if (d == (seq_t)1 && prev == (seq_t)4)
            return true;
        prev = d;
        x /= (seq_t)8;
    }
    return false;
}

/* Return the n-th element of sequence A044199 (1‑based index).  The
 * implementation scans integers on every call; it does not rely on
 * dynamic allocation, so it works with any seq_t type, including
 * arbitrary‑precision integer classes used in the C++ build. */
seq_t a(idx_t n)
{
    idx_t  i   = 0;       /* current integer being examined */
    idx_t  found = 0;     /* number of sequence terms found */

    while (found < n) {
        ++i;
        /* i is at least 1 because n ≥ 1 and we increment before use. */
        if (has_14((seq_t)i) && !has_14((seq_t)(i - 1))) {
            ++found;
            if (found == n)
                return (seq_t)i;
        }
    }

    /* The loop always returns; the following return is only to keep the
     * compiler happy. */
    return (seq_t)0;
}
