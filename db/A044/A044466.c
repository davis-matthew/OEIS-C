/*
 * A044466  Numbers n such that string 1,3 occurs in the base 5 representation
 *          of n but not of n+1.
 * https://oeis.org/A044466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A044466:  Numbers n whose base‑5 representation contains the
 *  substring "13" while the base‑5 representation of n+1 does not.
 *
 *  The function returns the n‑th such integer, a(1)=8, a(2)=33, …
 *  The computation is on‑the‑fly, so the function works for any
 *  build where seq_t is an arbitrary‑precision integer class or a
 *  builtin unsigned integer type.
 */
#include <stdbool.h>
#include <stdint.h>

/*  Check whether a base‑5 number has the substring 1 followed by 3
 *  (digits in normal order, most significant first).  */
static bool has13_base5(idx_t x)
{
    /* Scan two consecutive digits at a time – no extra storage needed */
    while (x >= 5) {
        if ((x % 5) == 3 && ((x / 5) % 5) == 1)
            return true;
        x /= 5;
    }
    return false;
}

/*  Public interface ----------------------------------------------------- */
seq_t a(idx_t n)
{
    idx_t count = 0;        /* How many numbers found so far */
    idx_t candidate = 1;    /* Current integer to test */

    while (count < n) {
        if (has13_base5(candidate) && !has13_base5(candidate + 1)) {
            count++;
            if (count == n)
                return (seq_t)candidate;   /* cast to seq_t (handles both
                                             * builtin and big‑int cases) */
        }
        candidate++;
    }

    /* This point should never be reached because the sequence is infinite */
    return (seq_t)0;
}
