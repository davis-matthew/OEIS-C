/*
 * A044367  Numbers n such that string 3,5 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044367
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

/* Method: brute‑force search; a(n) is the nth positive integer whose decimal
   representation contains substring "35" but (n-1) does not. */
#include <stdbool.h>

static bool has35(seq_t x)
{
    if (x < (seq_t)35) return false;
    int last = -1;
    while (x)
    {
        int cur = (int)(x % (seq_t)10);
        if (last == 5 && cur == 3) return true;
        last = cur;
        x /= (seq_t)10;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t cand = (seq_t)35;
    while (1)
    {
        if (has35(cand) && !has35(cand - (seq_t)1))
        {
            ++count;
            if (count == n)
                return cand;
        }
        cand++;
    }
}
