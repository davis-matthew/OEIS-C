/*
 * A044164  Numbers k such that string 3,5 occurs in the base 7 representation
 *          of k but not of k-1.
 * https://oeis.org/A044164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th integer k such that base‑7 representation of k
 *            contains the substring "35", but base‑7 representation of k-1
 *            does not. (brute‑force search)
 * Range:     u8 n <= 6, u16 n <= 1445, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 9576 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool has35(seq_t x)
{
    /* Quick reject for zero */
    if (x == 0)
        return false;

    seq_t last = x % 7;
    x /= 7;
    while (x > 0)
    {
        seq_t d = x % 7;
        if (d == 3 && last == 5)
            return true;
        last = d;
        x /= 7;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n=0, return 0 safely */

    seq_t k = 0;           /* candidate integer */
    idx_t found = 0;       /* how many suitable numbers found */
    bool prev_has = false; /* did k-1 contain "35"? */

    while (found < n)
    {
        ++k;                      /* next integer to test */
        bool cur_has = has35(k);  /* does k contain "35"? */

        if (cur_has && !prev_has) {
            ++found;
            if (found == n)
                return k;
        }
        prev_has = cur_has;
    }

    /* unreachable, kept for completeness */
    return (seq_t)0;
}
