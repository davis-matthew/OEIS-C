/*
 * A052421  Numbers without 8 as a digit.
 * https://oeis.org/A052421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = replace digits d>7 by d+1 in base-9 representation of n-1
 * Range:     u8 n <= 213, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If called with n==0, behaviour does not matter; return 0. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Step 1: convert n-1 to base 9 and map digits. */
    idx_t m = n - 1;
    seq_t result = (seq_t)0;
    seq_t multiplier = (seq_t)1;          /* 10^k for the current least significant digit */

    while (m > 0) {
        unsigned digit = (unsigned)(m % (idx_t)9);  /* current base‑9 digit (0–8) */
        m /= (idx_t)9;

        seq_t mapped = (digit == 8) ? (seq_t)9 : (seq_t)digit;
        result += mapped * multiplier;
        multiplier *= (seq_t)10;
    }

    return result;
}
