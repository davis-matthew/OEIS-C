/*
 * A169600  Numbers that are congruent to {4, 25} mod 31.
 * https://oeis.org/A169600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (62*n + 11*(-1)^n - 35) / 4 with a(1) = 4, a(2) = 25, …
 *            for n >= 1
 * Range:     u8 n <= 4, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 62*n */
    seq_t six2 = (seq_t)62 * n;

    /* determine the sign of the 11 term based on parity of n */
    seq_t numer;
    if ((n & 1ULL) == 0) {          /* n is even: +11 */
        numer = six2 + (seq_t)11 - (seq_t)35;
    } else {                        /* n is odd:  -11 */
        numer = six2 - (seq_t)11 - (seq_t)35;
    }

    /* divide by 4 – the numerator is always divisible by 4 */
    return numer / (seq_t)4;
}
