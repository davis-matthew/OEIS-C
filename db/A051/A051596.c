/*
 * A051596  Numerical values or Gematriot of Hebrew letters {aleph, bet, ...,
 *          tav}.
 * https://oeis.org/A051596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, word
 * Formula:   a(n) = (((n-1) mod 9)+1)*10^floor((n-1)/9)
 * Range:     u8 n <= 20, u16 n <= 22, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n < 1, return 0 */

    idx_t r = n - 1;              /* r = n-1 */
    seq_t digit = (seq_t)(r % 9) + 1;  /* digit in 1..9 */

    idx_t exp = r / 9;            /* exponent for the power of 10 */
    seq_t pow10 = (seq_t)1;       /* 10^exp */

    for (idx_t i = 0; i < exp; ++i)
        pow10 *= (seq_t)10;        /* multiply by 10 each step */

    return digit * pow10;          /* final term */
}
