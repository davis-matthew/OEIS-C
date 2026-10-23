/*
 * A044192  Numbers n such that string 0,5 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer such that its base‑8 representation *
 *            contains the substring "05" while that substring does not appear
 *            in * the base‑8 representation of (a(n) - 1)
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence has offset 1.
 * a(1) = 69, a(2) = 133, a(3) = 197, … */

#include <stdbool.h>
#include <stddef.h>

/* Return true iff the base‑8 representation of x contains the substring
 * "05".  For x < 8 the representation has a single digit and cannot
 * contain the substring.  We scan the digits from least significant to
 * most significant; a pair of consecutive digits is (curr, prev)
 * where prev is the digit to the left of curr in the usual string.  */
static bool has05(seq_t x)
{
    if (x < 8) return false;          /* one‑digit numbers cannot contain "05". */
    seq_t prev = x % 8;                /* least‑significant digit */
    x /= 8;
    while (x != 0) {
        seq_t curr = x % 8;            /* next digit to the left */
        if (prev == 5 && curr == 0)    /* pattern "0 5" in the normal order */
            return true;
        prev = curr;
        x /= 8;
    }
    return false;
}

/* Find the n‑th number satisfying the condition.  The search starts
 * at 0 and examines every integer in order, stopping when the desired
 * count is reached.  Each candidate is tested by calling has05 twice:
 * once with the candidate and once with (candidate - 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n = 0, harmless. */

    idx_t count  = 0;                   /* how many qualifying numbers found */
    seq_t cand   = 0;                   /* candidate to test */

    while (true) {
        ++cand;                         /* next integer to examine */

        if (has05(cand) && !has05(cand - 1)) {
            ++count;
            if ((idx_t)count == n)      /* found the n‑th term */
                return cand;
        }
    }
}
