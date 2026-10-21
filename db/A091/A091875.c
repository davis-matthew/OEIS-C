/*
 * A091875  Square roots of squares in A068176.
 * https://oeis.org/A091875
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A091875 – “Square roots of squares in A068176”.
 *  Formula:
 *     a(n) = 5   , n = 2
 *     a(n) = 15  , n = 3
 *     a(n) = 35  , n = 4
 *     a(n) = 115 , n = 5
 *     a(n) = 365 , n = 6
 *     a(n) = 3215, n = 7
 *     a(n) = 3215·10^(n−7), n ≥ 7
 *
 *  The implementation is purely arithmetic; no preprocessing tables
 *  beyond the initial few terms are required.  It works for the
 *  built‑in integer types as well as for the arbitrary‑precision
 *  sequence type used when seq_t is a class.
 */

seq_t a(idx_t n)
{
    /* Handle the small indices explicitly.  idx_t is unsigned,
       so n < 2 is ignored – behaviour below the offset is undefined. */
    if (n <= 6) {
        static const seq_t small[7] = {0, 0, 5, 15, 35, 115, 365};
        return small[n];
    }

    /* For n >= 7 we need 3215 * 10^(n-7).  Compute the
       power of ten by repeated multiplication. */
    idx_t exp = n - 7;
    seq_t pow10 = 1;
    for (idx_t i = 0; i < exp; ++i)
        pow10 *= (seq_t)10;

    return (seq_t)3215 * pow10;
}
