/*
 * A055050  Numbers of the form 4^i*(8*j+3) or 4^i*(8*j+7).
 * https://oeis.org/A055050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9949 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers of the form 4^i*(8*j+3) or 4^i*(8*j+7) (positive integers whose odd part is 3 or 7 mod 8) */
/* This implementation scans successive integers and tests the property directly. */

#include <stdbool.h>

/* Return true iff x can be written as 4^i*(8*j+3) or 4^i*(8*j+7). */
static bool is_special(seq_t x)
{
    if (x == 0)          /* zero is not in the sequence */
        return false;

    /* Remove all factors of 4 (i.e., 2^2).  If the exponent of 2 in x is odd,
       the loop stops earlier and the odd part will still be even and the test
       below will fail, which is correct. */
    while ((x & (seq_t)3) == 0)  /* x is divisible by 4 */
        x >>= 2;

    /* Now x is the odd part of the original number.  It must be 3 or 7 (mod 8). */
    seq_t r = x & (seq_t)7;      /* x % 8 */
    return r == (seq_t)3 || r == (seq_t)7;
}

seq_t a(idx_t n)
{
    /* a(1) is the first term of the sequence.  n==0 is unused. */
    if (n == 0)
        return 0;            /* defensively return 0, though the offset is 1 */

    idx_t count = 0;
    seq_t candidate = 1;
    seq_t result = 0;

    while (count < n) {
        if (is_special(candidate)) {
            ++count;
            result = candidate;
        }
        candidate += (seq_t)1;
    }

    return result;
}
