/*
 * A219251  Numbers such that the absolute difference of a pair of adjacent
 *          decimal digits is not prime.
 * https://oeis.org/A219251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers such that NOT all adjacent decimal digit differences are prime. */
/* In other words, a number is in the sequence if it has at least one pair of adjacent
   decimal digits whose absolute difference is NOT 2, 3, 5, or 7. */

#include <stdbool.h>

/* Return true if candidate contains at least one pair of adjacent digits
   with a non‑prime absolute difference. */
static bool valid_num(seq_t x)
{
    if (x < 10)                         /* numbers < 10 are excluded (offset 1) */
        return false;

    bool first = true;                  /* flag for the most significant digit */
    int last   = -1;                    /* previous digit (more significant)  */

    while (x > 0) {
        int d = (int)(x % 10);          /* current least‑significant digit */

        if (!first) {
            int diff = last - d;
            if (diff < 0) diff = -diff;

            /* If the difference is not 2,3,5, or 7, the number qualifies.
               We can return immediately. */
            if (diff != 2 && diff != 3 && diff != 5 && diff != 7)
                return true;
        }

        first = false;
        last  = d;
        x /= 10;
    }

    /* All adjacent differences were prime --> exclude the number. */
    return false;
}

/* Return the n-th element of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined by OEIS convention */

    idx_t  count   = 0;
    seq_t  cand    = 10;                 /* smallest member of the sequence */

    for (;;) {
        if (valid_num(cand)) {
            ++count;
            if (count == n)
                return cand;
        }
        ++cand;
    }
}
