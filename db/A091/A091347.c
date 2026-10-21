/*
 * A091347  a(n) = 6*4^n - 12*3^n + 7*2^n - 1.
 * https://oeis.org/A091347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*4^n - 12*3^n + 7*2^n - 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute base^exp for seq_t using exponentiation by squaring.
 * It works for built‑in integer types as well as for
 * the arbitrary‑precision seq_t class (provided it has +, -, *, operators). */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Powers of 4, 3 and 2 */
    seq_t pow4 = pow_seq((seq_t)4, n);
    seq_t pow3 = pow_seq((seq_t)3, n);
    seq_t pow2 = pow_seq((seq_t)2, n);

    /* a(n) = 6*4^n - 12*3^n + 7*2^n – 1 */
    seq_t res = (seq_t)6 * pow4
               - (seq_t)12 * pow3
               + (seq_t)7 * pow2
               - (seq_t)1;
    return res;
}
