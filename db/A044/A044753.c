/*
 * A044753  Numbers n such that string 4,0 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OHSIS A044753: Numbers n such that the decimal string "40" occurs in n
 * but not in n+1.  The function computes the n-th such number by
 * brute‑force search, which is sufficient for n up to about 10 000. */

#include <stdbool.h>

static inline bool has_40(seq_t x)
{
    seq_t prev = 10;          /* sentinel not a digit */
    while (x > 0)
    {
        seq_t cur = x % 10;
        if (prev == 0 && cur == 4) return true;
        prev = cur;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* unused, offset is 1 */
    idx_t count = 0;
    idx_t candidate = 1;

    for (;; ++candidate)
    {
        seq_t val = (seq_t)candidate;
        if (has_40(val) && !has_40(val + 1))
        {
            ++count;
            if (count == n) return val;
        }
    }
}
