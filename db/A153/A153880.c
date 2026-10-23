/*
 * A153880  Shift factorial base representation left by one digit.
 * https://oeis.org/A153880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 8645, u32 n <= 9912, u64 n <= 9912,
 *            u128 n <= 9912, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Shift factorial base representation left by one digit.
   The factor base for each position i uses base (i+1): 0 ≤ digit < i+1.
   a(n) = Σ_{i≥1} digit_i * (i+1)! where digit_i are the digits of n
   in factorial base (least significant digit corresponds to 1!). */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t fac = 2;          /* (1+1)! = 2! */
    idx_t i = 1;

    while (n > 0)
    {
        idx_t d = n % (i + 1);            /* digit for i-th position */
        result += (seq_t)d * fac;         /* add contribution with (i+1)! */
        n = (n - d) / (i + 1);            /* value for next higher position */
        fac *= (seq_t)(i + 2);            /* (i+2)! for next loop */
        ++i;
    }

    return result;
}
