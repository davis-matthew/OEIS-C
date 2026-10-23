/*
 * A044956  Numbers with no two equally numerous base 7 digits.
 * https://oeis.org/A044956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: Numbers with no two equally numerous base‑7 digits */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Check whether a number satisfies the condition.
   The counts of the base‑7 digits 0…6 must be all distinct
   when considered only for positive counts. */
static bool valid_number(seq_t x)
{
    unsigned char cnt[7] = {0};
    while (x != 0) {
        unsigned d = (unsigned)(x % 7);
        cnt[d]++;
        x = x / 7;
    }
    /* Note: a count of 0 means the digit does not appear.
       Only positive counts are considered, and any two
       positive counts must be different. */
    unsigned char seen[256] = {0};
    for (int i = 0; i < 7; ++i) {
        unsigned char c = cnt[i];
        if (c == 0)
            continue;
        if (seen[c])
            return false;
        seen[c] = 1;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for n = 0; not used */
    seq_t candidate = 0;
    idx_t found = 0;
    while (found < n) {
        ++candidate;
        if (valid_number(candidate))
            ++found;
    }
    return candidate;
}
