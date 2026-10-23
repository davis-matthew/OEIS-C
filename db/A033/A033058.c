/*
 * A033058  Numbers whose base-7 representation Sum_{i=0..m} d(i)*7^i has odd
 *          d(m-i) for all odd i.
 * https://oeis.org/A033058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   valid_base7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: Generate the nth positive integer whose base‑7 digits satisfy
 *   : For every odd i, the digit at position (m-i) is odd
 *   : equivalently, in the L‑digit base‑7 representation (L = m+1),
 *     digits at positions j with parity different from L-1 are odd.
 * The function returns the nth term (a(1)=1) for all n ≥ 1.
 */

#include <stdbool.h>
#include "../utils/valid_base7.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* offset is 1, not used */

    seq_t  candidate = 1;
    idx_t  count     = 0;

    while (1)
    {
        if (valid_base7(candidate))
        {
            ++count;
            if (count == n) return candidate;
        }
        ++candidate;
    }
}
