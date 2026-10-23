/*
 * A044438  Numbers n such that string 1,2 occurs in the base 3 representation
 *          of n but not of n+1.
 * https://oeis.org/A044438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A044438 - Numbers n such that the base-3 representation of n contains
   the substring "12" while the base-3 representation of n+1 does not contain
   it.  The function returns the nth term of this sequence (offset 1).
   The implementation scans successive integers and checks the two
   conditions directly; it does not rely on any hard‑coded values.
*/

#include <stdbool.h>

/* Check whether the base‑3 representation of x contains the
   substring "12". */
static bool contains12(seq_t x)
{
    unsigned prev = 3;              /* value outside {0,1,2} to indicate no previous digit */
    while (x > 0)
    {
        unsigned d = (unsigned)(x % 3);
        if (d == 1 && prev == 2)
            return true;
        prev = d;
        x /= 3;
    }
    return false;
}

/* Returns a(n) for n >= 1.  The function works for any order of
   calls; it recomputes the value from scratch each time. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t count = 0;                /* number of qualifying terms found so far */
    seq_t cur = 1;                  /* current integer being examined */

    while (count < n)
    {
        if (contains12(cur) && !contains12(cur + 1))
        {
            ++count;
            if (count == n)
                return cur;
        }
        ++cur;
    }
    return 0;   /* unreachable */
}
