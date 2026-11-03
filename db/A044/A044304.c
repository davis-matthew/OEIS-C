/*
 * A044304  Numbers n such that string 5,8 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A044304:
   Numbers n such that the base‑9 representation of n contains the contiguous
   substring “58” but the base‑9 representation of n–1 does not.
   The function is implemented by a simple incremental search. */

#include <stdbool.h>

/* Return true if x in base 9 contains the pattern 5 followed by 8. */
static bool has_58(seq_t x)
{
    unsigned char digits[32];
    int len = 0;

    if (x == 0)
        return false;            /* empty representation has no 58 */

    while (x > 0) {
        digits[len++] = (unsigned char)(x % 9);
        x /= 9;
    }

    /* Scan from most significant to least significant digit. */
    for (int i = len - 1; i > 0; --i) {
        if (digits[i] == 5 && digits[i - 1] == 8)
            return true;
    }
    return false;
}

/* Return the n-th term of A044304 (n >= 1). */
seq_t a(idx_t n)
{
    seq_t candidate = 0;
    idx_t count = 0;

    while (count < n) {
        ++candidate;                       /* try next integer            */
        if (has_58(candidate) && !has_58(candidate - 1))
            ++count;
    }
    return candidate;
}
