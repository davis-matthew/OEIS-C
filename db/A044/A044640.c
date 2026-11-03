/*
 * A044640  Numbers k such that string 0,8 occurs in the base 9 representation
 *          of k but not of k+1.
 * https://oeis.org/A044640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that the base‑9 representation of k contains the
 * substring "08" while the base‑9 representation of k+1 does not.
 * The sequence is defined by searching for such k in increasing order. */

#include <stdbool.h>
#include <stddef.h>

/* Return true if the base‑9 digits of n contain a "0" immediately
 * followed by an "8". */
static bool has08(seq_t n)
{
    if (n == 0)
        return false;

    unsigned char d[64];
    size_t len = 0;
    seq_t t = n;

    /* Write digits in reverse order (least significant first). */
    while (t != 0) {
        d[len++] = (unsigned char)(t % 9);
        t /= 9;
    }

    if (len < 2)
        return false;

    /* Scan forward (most significant → least significant) looking
     * for 0 followed by 8. */
    for (size_t i = len; i-- > 1;) {
        if (d[i] == 0 && d[i - 1] == 8)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    seq_t k = 0;      /* current candidate value          */
    idx_t count = 0;  /* number of valid terms found so far */

    while (1) {
        k++;                               /* next integer to test     */
        if (has08(k) && !has08(k + 1)) {
            count++;                       /* we found a term of the sequence */
            if (count == n)
                return k;                  /* the n‑th term is found              */
        }
    }
}
