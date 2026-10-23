/*
 * A039311  Numbers whose base-7 representation has the same nonzero number of
 *          0's and 4's.
 * https://oeis.org/A039311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Search for numbers whose base‑7 representation has the same 
   number of 0’s and 4’s, and that number of each digit is non‑zero. 
   The function scans integers in increasing order until it finds the 
   n‑th such number. 
*/

#include <stdbool.h>

static inline bool has_equal_zero_four(seq_t x)
{
    unsigned zeros = 0, fours = 0;
    while (x != 0) {
        unsigned d = (unsigned)(x % (seq_t)7);
        if (d == 0) zeros++;
        else if (d == 4) fours++;
        x /= (seq_t)7;
    }
    return zeros == fours && zeros > 0;
}

seq_t a(idx_t n)
{
    idx_t count     = 0;
    seq_t candidate = 1;
    for (;;) {
        if (has_equal_zero_four(candidate)) {
            count++;
            if (count == n)
                return candidate;
        }
        candidate++;
    }
}
