/*
 * A040302  Continued fraction for sqrt(320).
 * https://oeis.org/A040302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0) = 17, a(1) = 1. For n > 0: a(n) = 1 if n is odd; a(n) = 7
 *            if n is divisible by 2 but not by 4; otherwise a(n) = 34
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative definition: a(2) = 7, a(2^e) = 34 (e >= 2), a(p^e) = 1 for odd primes p. */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)17;
    /* n >= 1 */
    if (n & (idx_t)1)
        return (seq_t)1;           /* odd n */

    /* n is even */
    /* power of two check: only one bit set? */
    if ((n & (n - (idx_t)1)) == 0) {
        /* n is a power of two */
        if (n == (idx_t)2)
            return (seq_t)7;
        else
            return (seq_t)34;     /* 2^e with e >= 2 */
    } else {
        /* contains odd factors */
        if ((n & (idx_t)3) == 0)
            return (seq_t)34;     /* divisible by 4 */
        else
            return (seq_t)7;      /* exactly one factor of 2 */
    }
}
