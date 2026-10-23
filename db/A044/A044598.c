/*
 * A044598  Numbers n such that string 3,6 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer n such that the octal representation of n
 *            contains the substring "36", but the octal representation of n+1
 *            does not
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* test whether a number x contains the substring "36" in base 8 */
static bool has36(seq_t x)
{
    seq_t t = x;
    if (t == 0)
        return false;
    int prev = -1;
    while (t > 0)
    {
        unsigned int cur = (unsigned int)(t % (seq_t)8);
        if (prev == 6 && cur == 3)
            return true;
        prev = cur;
        t = t / (seq_t)8;
    }
    return false;
}

/* OEIS sequence A044598 */
seq_t a(idx_t n)
{
    if (n == 0)
        abort();            /* sequence is 1‑based (offset 1) */

    idx_t count = 0;
    seq_t candidate = (seq_t)1;
    while (count < n)
    {
        if (has36(candidate) && !has36(candidate + (seq_t)1))
        {
            ++count;
            if (count == n)
                return candidate;
        }
        candidate += (seq_t)1;
    }
    return candidate;      /* should never reach here */
}
