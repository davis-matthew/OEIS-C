/*
 * A037341  Numbers whose base-2 and base-7 expansions have no digits in
 *          common.
 * https://oeis.org/A037341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth integer whose base‑7 expansion contains only the
 *            digits 2–6 (equivalently, its base‑2 and base‑7 representations
 *            share no digits)
 * Range:     u8 n <= 105, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true if all base‑7 digits of x are ≥ 2. */
static inline bool good_base7(seq_t x)
{
    while (x > 0)
    {
        seq_t d = x % (seq_t)7;
        if (d < (seq_t)2)
            return false;
        x /= (seq_t)7;
    }
    return true;
}

seq_t a(idx_t n)
{
    seq_t target = (seq_t)n;      /* n is 1‑based */
    seq_t count  = 0;
    seq_t cand   = 2;             /* the first valid number */

    while (count < target)
    {
        if (good_base7(cand))
        {
            count++;
            if (count == target)
                return cand;
        }
        cand += (seq_t)1;
    }

    /* Should never reach here */
    return cand;
}
