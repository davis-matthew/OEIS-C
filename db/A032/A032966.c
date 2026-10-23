/*
 * A032966  Numbers whose base-16 representation Sum_{i=0..m} d(i)*16^(m-i)
 *          has even d(i) for all odd i.
 * https://oeis.org/A032966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th positive integer whose hexadecimal representation
 *            has even digits at all odd positions (positions counted from 0
 *            at the most significant digit)
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Helper: test whether a number satisfies the digit condition.
   For each digit at odd position (from most significant), the digit
   must be even. */
static inline bool satisfies(seq_t x)
{
    /* Determine the number of hex digits. */
    idx_t len = 0;
    seq_t tmp = x;
    do {
        tmp >>= 4;          /* divide by 16 */
        ++len;
    } while (tmp != 0);

    /* Inspect digits from least significant to most. */
    seq_t y = x;
    for (idx_t k = 0; k < len; ++k) {
        uint8_t digit = (uint8_t)(y & 0xF);  /* current hex digit */
        idx_t pos = len - 1 - k;              /* position from most sig. */
        if ((pos & 1) == 1) {                 /* odd position? */
            if (digit & 1) return false;      /* digit is odd → reject */
        }
        y >>= 4;
    }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t candidate = 1;
    idx_t count = 0;
    while (1) {
        if (satisfies(candidate)) {
            ++count;
            if (count == n) return candidate;
        }
        ++candidate;
    }
}
