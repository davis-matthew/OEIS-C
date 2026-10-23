/*
 * A155098  Numbers k such that k^2 == -1 (mod 41).
 * https://oeis.org/A155098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*(-1)^(n+1) + 41*floor(n/2)
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Ensure n >= 1; the behavior for n < 1 is undefined but
       the caller will not use such values per the problem statement. */

    seq_t half = (seq_t)(n / 2);          /* floor(n/2) */
    seq_t result = 41 * half;            /* 41 * floor(n/2) */

    /* Add or subtract 9 depending on the parity of n. */
    if (n & 1U)                          /* n is odd -> (+9) */
        result += 9;
    else                                 /* n is even -> (-9) */
        result -= 9;

    return result;
}
