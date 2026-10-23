/*
 * A044467  Numbers n such that string 1,4 occurs in the base 5 representation
 *          of n but not of n+1.
 * https://oeis.org/A044467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A044467 (numbers n whose base‑5
   representation contains the substring "14" but n+1 does not).
   The implementation performs a brute‑force search recording
   successive valid numbers.  This method is fast enough for n ≤ 10000. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Return true iff the base‑5 representation of x contains the
   consecutive digits 1 followed by 4. */
static bool has14(seq_t x)
{
    if (x == 0) return false;                /* Base‑5 "0" has no "14". */
    uint8_t digits[128];                     /* Enough for 128‑bit values. */
    size_t len = 0;
    while (x != 0) {
        digits[len++] = (uint8_t)(x % 5);    /* Store least significant digit. */
        x /= 5;
    }
    for (size_t i = 1; i < len; ++i)
        if (digits[i] == 1 && digits[i - 1] == 4)
            return true;
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* Undefined for index 0, but safe. */
    idx_t count = 0;
    seq_t m = 0;
    for (;;) {
        ++m;                                 /* Next candidate. */
        if (has14(m)) {
            seq_t mp1 = m + 1;
            if (!has14(mp1)) {
                ++count;
                if (count == n) return m;
            }
        }
    }
}
