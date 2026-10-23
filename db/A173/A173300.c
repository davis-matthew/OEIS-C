/*
 * A173300  a(n) is the denominator of the fraction f = x^n + y^n given that x
 *          + y = 1 and x^2 + y^2 = 2.
 * https://oeis.org/A173300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(1) = 1, a(2) = 1. For n ≥ 3: if n is odd: a(n) = 2^((n‑1)/2)
 *            if n ≡ 0 (mod 4): a(n) = 2^(n/2 − 1) if n ≡ 2 (mod 4): a(n) =
 *            2^(n/2 − 2) (This equals the recurrence a(n) = 4·a(n‑4) for n ≥
 *            7.)
 * Range:     u8 n <= 16, u16 n <= 32, u32 n <= 64, u64 n <= 128,
 *            u128 n <= 256, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms. */
    if (n < 3) return 1;

    /* Compute the exponent according to the parity of n. */
    idx_t exp;
    if (n & 1U) {                     /* n is odd            */
        exp = (n - 1U) / 2U;
    } else if ((n & 3U) == 0U) {       /* n ≡ 0 mod 4         */
        exp = n / 2U - 1U;
    } else {                           /* n ≡ 2 mod 4         */
        exp = n / 2U - 2U;
    }

    /* Compute 2^exp.  Use repeated multiplication to avoid
       shifting beyond the width of seq_t on finite-width types. */
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i) {
        res *= 2;
    }
    return res;
}
