/*
 * A135521  a(n) = 2^(A091090(n)) - 1.
 * https://oeis.org/A135521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 2^(A091090(n)) - 1. Recurrence: a(1) = 1. For n > 1: if n
 *            is even, a(n) = 1; if n is odd, a(n) = 1 + 2*a((n-1)/2)
 * Range:     u8 n <= 510, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 116 terms through n = 9756 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;
    if ((n & 1) == 0)
        return 1;
    return 1 + 2 * a((n - 1) >> 1);
}
