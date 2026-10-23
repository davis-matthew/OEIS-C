/*
 * A031984  Numbers with exactly five distinct base-7 digits.
 * https://oeis.org/A031984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Numbers with exactly five distinct base‑7 digits (search) */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Offset 1 – the first term is a(1) */
    if (n == 0) return 0;   /* behaviour below the offset is irrelevant */

    seq_t count = 0;
    seq_t val   = (seq_t)2401;   /* smallest 5‑digit base‑7 number (7^4) */

    for (;; ++val)
    {
        unsigned mask = 0;
        int distinct  = 0;
        seq_t tmp = val;

        while (tmp)
        {
            unsigned d   = (unsigned)(tmp % 7);
            unsigned bit = 1u << d;
            if (!(mask & bit))
            {
                mask |= bit;
                ++distinct;
                if (distinct > 5) break;   /* early exit on overflow */
            }
            tmp /= 7;
        }

        if (distinct == 5)
        {
            ++count;
            if (count == n) return val;
        }
    }
}
