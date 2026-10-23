/*
 * A044469  Numbers n such that string 2,1 occurs in the base 5 representation
 *          of n but not of n+1.
 * https://oeis.org/A044469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers n such that the string "21" occurs in the base‑5
 *            representation of n but does not occur in the base‑5
 *            representation of n+1
 * Range:     u8 n <= 12, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Check whether x contains the digit pattern 2,1 in its base‑5 representation. */
static inline bool has21(seq_t x)
{
    if (x < (seq_t)5)                /* need at least two digits */
        return false;

    seq_t prev = x % (seq_t)5;       /* least significant digit */
    seq_t y = x / (seq_t)5;          /* remaining higher digits */

    while (y > (seq_t)0)
    {
        seq_t cur = y % (seq_t)5;    /* next significant digit */
        if (cur == (seq_t)2 && prev == (seq_t)1)
            return true;
        prev = cur;
        y /= (seq_t)5;
    }
    return false;
}

/* Return the nth term (n ≥ 1) of A044469. */
seq_t a(idx_t n)
{
    if (n == 0)                      /* undefined, but keep to avoid UB */
        return 0;

    idx_t cnt = 0;
    seq_t val = 0;

    while (1)
    {
        if (has21(val) && !has21(val + (seq_t)1))
        {
            cnt++;
            if (cnt == n)
                return val;
        }
        val++;
    }
}
