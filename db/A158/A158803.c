/*
 * A158803  Numbers k such that k^2 == 2 (mod 41).
 * https://oeis.org/A158803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A158803: Numbers k such that k^2 == 2 (mod 41)
   Formula: if n is odd, a(n) = 17 + 41 * floor((n-1)/2);
            if n is even, a(n) = 24 + 41 * (floor(n/2) - 1);
   (This directly uses the congruence that the terms are 17 or 24 mod 41.) */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* offset starts at 1, but guard for safety */

    seq_t q = (seq_t)n / 2;     /* floor(n/2) */
    if (n & 1)                 /* odd n */
        return (seq_t)17 + (seq_t)41 * q;
    else                       /* even n */
        return (seq_t)24 + (seq_t)41 * (q - 1);
}
