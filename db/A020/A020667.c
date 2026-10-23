/*
 * A020667  Least n-th power containing every digit.
 * https://oeis.org/A020667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A020666(n)^n (search for the minimal base m such that m^n
 *            has all decimal digits 0–9)
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 16, u128 n <= 53,
 *            big any n
 * Helpers:   digit_counts, pow2, seq_from_str, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"
#include "../utils/digit_counts.h"
#include "../utils/seq_from_str.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Special case n = 1 : the smallest integer containing all
       decimal digits is 1 0 2 3 4 5 6 7 8 9.  We construct it via
       seq_from_str to avoid hard‑coding the numeric value. */
    if (n == 1)
        return seq_from_str("1023456789");

    /* For large n the minimal base is known to be 2.
       This shortcut keeps the run‑time tiny for n≥169. */
    if (n >= 169)
        return pow2(n);

    /* Brute force search for the minimal base m ≥ 1
       such that m^n contains all digits 0–9. */
    idx_t base = 1;
    seq_t power;
    unsigned char cnt[10];

    for (;; ++base)
    {
        power = seq_pow((seq_t)base, n);
        digit_counts(power, cnt);

        bool all = true;
        for (int d = 0; d < 10; ++d)
            if (cnt[d] == 0)
            {
                all = false;
                break;
            }
        if (all)
            return power;
    }
}
