/*
 * A044056  Numbers n such that string 1,1 occurs in the base 3 representation
 *          of n but not of n-1.
 * https://oeis.org/A044056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-based implementation: a(n) is the n‑th positive integer x such that
   the base‑3 representation of x contains the substring "11" while the base‑3
   representation of x-1 does not. */

#include <stdbool.h>

/* Return true iff the base‑3 representation of n contains the substring "11". */
static bool has_11(seq_t n)
{
    if (n == 0) return false;
    bool first = true;
    unsigned int prev = 0, cur;
    while (n > 0) {
        cur = (unsigned int)(n % 3);
        if (!first) {
            if (cur == 1 && prev == 1) return true;
        }
        prev = cur;
        first = false;
        n /= 3;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Indexing starts at 1; behaviour for n==0 is undefined in OEIS. */
    if (n == 0) return 0;

    seq_t x = 1;           /* candidate number */
    idx_t  count = 0;       /* how many qualifying numbers seen */

    while (1) {
        bool h   = has_11(x);
        bool hprev = has_11(x - 1);   /* safe because x>=1 */

        if (h && !hprev) {
            count++;
            if (count == n) return x;/* found the n‑th term */
        }
        x++;                            /* next integer */
    }
}
